/*
 * Parameters.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: salwatorska
 */

#include "Parameters.h"
#include <stdio.h>

Parameters::Parameters() {
}

Parameters& Parameters::getInstance(char* query){
	parseQuery(query);
	return getInstance();
}

Parameters& Parameters::getInstance(){
	return instance;
}

int Parameters::parseQuery(char* query) {
	if (query == NULL) query = (char*)"";
	sscanf(query,"serial=%[^'&']&data=%s",instance.serial,instance.data);
	return 0;  // TODO: add checking here
}

const char* const Parameters::getSerialPort(){
	return serial;
}

Parameters::~Parameters() {
	// TODO Auto-generated destructor stub
}

