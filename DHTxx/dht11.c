/*
 *  DHT11/DHT22 temparature/humidity sensor reading application for TP-LINK TL-WDR4300 router running 
 *  OpenWRT (OpenWrt 15.05 Linux Kernel 3.18.14)
 *  
 *  Base on: 
 *   - Adafruit_Python_DHT by Tony DiCola (https://github.com/adafruit/Adafruit_Python_DHT)
 *   - Technion DHT22 reader (https://github.com/technion/lol_dht22/blob/master/dht22.c)
 *      dht22.c:
 *
 *	Contact: Marcin Bajer (bajer.mar@gmail.com or https://github.com/bajerwitharm/smartbuilding) 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>
#include <sys/resource.h>
#include <sys/time.h>

// This is the only processor specific magic value, the maximum amount of time to
// spin in a loop before bailing out and considering the read a timeout.  This should
// be a high value, but if you're running on a much faster platform than a Raspberry
// Pi or Beaglebone Black then it might need to be increased.
#define DHT_MAXCOUNT 30000

// Number of bit pulses to expect from the DHT.  Note that this is 41 because
// the first pulse is a constant 50 microsecond pulse, with 40 pulses to represent
// the data afterwards.
#define DHT_PULSES 41

enum pin_direction_e {
	OUTPUT = 0,
	INPUT = 1
};

enum pin_state_e {
	LOW = 0,
	HIGH = 1
};

enum sensor_type_e {
	DHT11 = 11,
	DHT22 = 22
};

enum read_result_e {
	DHTxx_READ_OK = 0,
	DHTxx_READ_TIMEOUT = 1,
	DHTxx_READ_WRONG_CRC = 2,
	GPIO_INIT_FAILED = 3
};


static enum sensor_type_e type = DHT11;
static int DHTPIN = 2;


#define GPIO_ADDR 0x18040000 // base address
#define GPIO_BLOCK 108 // memory block size

volatile unsigned long *gpioAddress;

int gpioSetup()
{
    int  m_mfd;
    if ((m_mfd = open("/dev/mem", O_RDWR)) < 0)
    {
        return -1;
    }
    gpioAddress = (unsigned long*)mmap(NULL, GPIO_BLOCK, PROT_READ|PROT_WRITE, MAP_SHARED, m_mfd, GPIO_ADDR);
    close(m_mfd);

    if (gpioAddress == MAP_FAILED)
    {
        return -2;
    }
    // disables special functions of some GPIO pins 
	//  *(gpioAddress + 10)=0x00008001;
    return 0;
}

void pinMode(int gpio, enum pin_direction_e direction)
{
    unsigned long value = *(gpioAddress); // obtain current settings
    if (direction == INPUT)
    {
        value |= (1 << gpio); // set bit to 1
    }
    else
    {
        value &= ~(1 << gpio); // clear bit
    }
    *(gpioAddress) = value;
}

void digitalWrite(int gpio, enum pin_state_e value)
{
    if (value == LOW)
    {
	*(gpioAddress + 4) = (0x00000001 << gpio);
    }
    else
    {
	*(gpioAddress + 3) = (0x00000001 << gpio);
    }
}

static int digitalRead(int gpio)
{
    unsigned long value = *(gpioAddress + 1);
    return (value & (1 << gpio));
}

static enum read_result_e read_dht22_dat(int DHTPIN, enum sensor_type_e type)
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0;
  int i;

  int pulseCounts[DHT_PULSES*2] = {0};
  int dht22_dat[5] = {0,0,0,0,0};

  // pull pin down for 20 milliseconds
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, HIGH);
  usleep(50000);
  digitalWrite(DHTPIN, LOW);
  // increase priority of process
  setpriority(PRIO_PROCESS, 0, -20);
  usleep(20000);
  // then pull it up for short time
  digitalWrite(DHTPIN, HIGH);

  // prepare to read the pin
  pinMode(DHTPIN, INPUT);

  // Wait for DHT to pull pin low.
  uint32_t count = 0;
  while (digitalRead(DHTPIN)) {
    if (++count >= DHT_MAXCOUNT) {
      // Timeout waiting for response.
      return DHTxx_READ_TIMEOUT;
    }
  }

  // Record pulse widths for the expected result bits.
  for (i=0; i < DHT_PULSES*2; i+=2) {
    // Count how long pin is low and store in pulseCounts[i]
    while (!digitalRead(DHTPIN)) {
      if (++pulseCounts[i] >= DHT_MAXCOUNT) {
        return DHTxx_READ_TIMEOUT;
      }
    }
    // Count how long pin is high and store in pulseCounts[i+1]
    while (digitalRead(DHTPIN)) {
      if (++pulseCounts[i+1] >= DHT_MAXCOUNT) {
        return DHTxx_READ_TIMEOUT;
      }
    }
  }
  // Done with timing critical code, now interpret the results.
  setpriority(PRIO_PROCESS, 0, 0);

  // Go to default output state
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, HIGH);


  // Compute the average low pulse width to use as a 50 microsecond reference threshold.
  // Ignore the first two readings because they are a constant 80 microsecond pulse.
  uint32_t threshold = 0;
  for (i=2; i < DHT_PULSES*2; i+=2) {
    threshold += pulseCounts[i];
  }
  threshold /= DHT_PULSES-1;

  // Interpret each high pulse as a 0 or 1 by comparing it to the 50us reference.
  // If the count is less than 50us it must be a ~28us 0 pulse, and if it's higher
  // then it must be a ~70us 1 pulse.
  uint8_t data[5] = {0};
  for (i=3; i < DHT_PULSES*2; i+=2) {
    int index = (i-3)/16;
    data[index] <<= 1;
    if (pulseCounts[i] >= threshold) {
      // One bit for long pulse.
      data[index] |= 1;
    }
    // Else zero bit for short pulse.
  }

  // printf("Data: 0x%x 0x%x 0x%x 0x%x 0x%x\n", data[0], data[1], data[2], data[3], data[4]);
  // Verify checksum of received data.
  if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
	float humidity;
	float temperature;
  
    if (type == DHT11) {
      // Get humidity and temp for DHT11 sensor.
      humidity = (float)data[0];
      temperature = (float)data[2];
    }
    else if (type == DHT22) {
      // Calculate humidity and temp for DHT22 sensor.
      humidity = (data[0] * 256 + data[1]) / 10.0f;
      temperature = ((data[2] & 0x7F) * 256 + data[3]) / 10.0f;
      if (data[2] & 0x80) {
        temperature *= -1.0f;
      }
    }
    printf("{\ntemp: %f,\nhumidity: %f\n}",temperature, humidity);
    return DHTxx_READ_OK;
  }
  else {
    return DHTxx_READ_WRONG_CRC;
  }
}


/**
 * Main function
 * Program parameters:
 *  <pin> - data pin of connected sensor 
 *  <sensor_type> - 11 for DHT11, 22 for DHT22
 */
int main (int argc, char *argv[])
{
  int i=0;
  int dhtpin;
  enum sensor_type_e sensor_type;
  if (argc != 3)
  {
	printf ("Usage: %s <pin> <sensor_type> \nDescription: pin is the DATA pin number of DHTxx sensor, sensor_type is xx in DHTxx\n",argv[0]);
  }
  else
  {
  	dhtpin = atoi(argv[1]);
	sensor_type = (enum sensor_type_e) atoi(argv[2]);
  }
  if (gpioSetup() != 0) {
  	perror("GPIO init failed");
	return GPIO_INIT_FAILED; 
  };
	
  if (setuid(getuid()) < 0)
  {
    perror("Dropping privileges failed\n");
    exit(EXIT_FAILURE);
  }

  if (read_dht22_dat(dhtpin, sensor_type) != DHTxx_READ_OK){
	sleep(2);
	return read_dht22_dat(dhtpin, sensor_type);
  };
}
