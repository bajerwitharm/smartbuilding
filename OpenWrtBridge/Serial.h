/*
 * Serial.h
 *
 *  Created on: Feb 2, 2015
 *      Author: salwatorska
 */

#ifndef SERIAL_H_
#define SERIAL_H_

class Serial {
public:
	Serial();
	virtual ~Serial();
	int closePort();
	int openPort(const char port[] = "/dev/ttyUSB0");
private:
	int handle;
};

#endif /* SERIAL_H_ */
