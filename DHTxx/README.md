# DHT11/DHT22 reading by OpenWRT based router
## Abstract
The goal of this document is to describe implementation of reading [DHTxx](http://www.micropik.com/PDF/dht11.pdf) temperature/humidity sensor by [TP-Link TL-WDR4300](http://wiki.openwrt.org/toh/tp-link/tl-wdr4300) router running OpenWRT. 
* To compile software it is required to have toolchain for the [Atheros AR9344](http://cykey.ca/~cykey/airport/AR9344.pdf) (OpenWRT prefered). 
* Software is not a driver, all calculations are performed in user space. Unfortunetely, this is not very optimal solution since waiting for DATA change pin are done as ***while*** loop. To increase accuracy process priority is increased to highest during the time critical part.

## DHTxx communication protocol
[DHT11](http://www.micropik.com/PDF/dht11.pdf) is a 4 pin sensor which can measure temperatures ranging from 0~50°C with accuracy of 1°C & relative humidity ranging from 20-95%. [DHT22](https://www.sparkfun.com/datasheets/Sensors/Temperature/DHT22.pdf) has better characteristics - humidity 0~100%RH; temperature -40~80°C with accuracy of 0.5°C. The sensor uses its own proprietary 1-wire protocol to communicate with master device. 

## Sensor connection
The only PIN which is not used by the [TL-WDR4300](http://wiki.openwrt.org/toh/tp-link/tl-wdr4300) router is located at pin 5 of [JTAG (JP1)](http://wiki.openwrt.org/toh/tp-link/tl-wdr4300#gpio) slot. This pin is connected to GPIO 2. The sensor can be connected directly without pull-up resistor. Other pins can be used if special purpose of GPIO will be disabled (see initialization of GPIO). 3,3V power supply can be taken from [Serial port](http://wiki.openwrt.org/toh/tp-link/tl-wdr4300#serial_console) slot. 

## Customization to other routers
Generally this software should work without problems on other routers. It is required to customize GPIO registers addresses to support used uP.

## Usage
**./dht11** ***pin*** ***sensor_type***

Description: ***pin*** is the router's GPIO pin number to which DATA pin of DHTxx sensor is connected, ***sensor_type*** is xx in DHTxx

i.e.

**./dht11** ***2*** ***11***

output is JSON coded string:

```
{
temp: 28.000000,
humidity: 31.000000
}
```

In addition if [luci](http://wiki.openwrt.org/doc/howto/luci.essentials) is installed and [gettemp](https://github.com/bajerwitharm/smartbuilding/blob/master/DHTxx/gettemp) file is copied to ***/www/cgi-bin*** script results can be accessed via web browser at address ***http://router-ip/cgi-bin/gettemp***



