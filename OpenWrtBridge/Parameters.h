/*
 * Parameters.h
 *
 *  Created on: Feb 2, 2015
 *      Author: salwatorska
 */

#ifndef PARAMETERS_H_
#define PARAMETERS_H_

class Parameters {
public:
	static Parameters& getInstance();
	static Parameters& getInstance(char* query);
	const char* const getSerialPort();
private:
	static Parameters instance;
	Parameters();
	virtual ~Parameters();
	int parseQuery(char* query);
	char serial[25] = "/dev/ttyUSB0";
	char data[255] = {};
};

#endif /* PARAMETERS_H_ */
