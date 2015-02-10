/*
 * Serial.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: salwatorska
 */

#include "Serial.h"
#include <unistd.h>     // UNIX standard function definitions
#include <fcntl.h>      // File control definitions
#include <errno.h>      // Error number definitions
#include <termios.h>    // POSIX terminal control definitions



Serial::Serial() {
	// TODO Auto-generated constructor stub

}

Serial::~Serial() {
	// TODO Auto-generated destructor stub
}

int Serial::closePort() {
	return close(handle);
}

int Serial::openPort(const char port[]) {
	/* Open File Descriptor */
	int USB = open(port, O_RDWR | O_NONBLOCK | O_NDELAY);
	/* Error Handling */
	if (USB < 0) {
		printf("Error opening port %d (%s)", errno, strerror(errno));
		return -1;
	}
	/* *** Configure Port *** */
	struct termios tty;
	memset(&tty, 0, sizeof tty);
	/* Error Handling */
	if (tcgetattr(USB, &tty) != 0) {
		printf("Error getting port attributes %d (%s)", errno, strerror(errno));
		return -1;
	}
	/* Set Baud Rate */
	cfsetospeed(&tty, B9600);
	cfsetispeed(&tty, B9600);
	/* Setting other Port Stuff */
	tty.c_cflag &= ~PARENB;
	tty.c_cflag &= ~CSTOPB;
	tty.c_cflag &= ~CSIZE;
	tty.c_cflag |= CS8;
	tty.c_cflag &= ~CRTSCTS;
	tty.c_lflag = 0; // no signaling chars, no echo, no canonical processing
	tty.c_oflag = 0; // no remapping, no delays
	tty.c_cc[VMIN] = 0;
	tty.c_cc[VTIME] = 5;
	tty.c_cflag |= CREAD | CLOCAL;
	tty.c_iflag &= ~(IXON | IXOFF | IXANY);
	tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	tty.c_oflag &= ~OPOST;
	/* Flush Port, then applies attributes */
	tcflush(USB, TCIFLUSH);
	if (tcsetattr(USB, TCSANOW, &tty) != 0) {
		printf("Error while nie wiem co %d (%s)", errno, strerror(errno));
		return -1;
	}
	handle = USB;
	return 0;
}

