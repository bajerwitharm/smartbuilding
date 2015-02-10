/*
 * UrlParamConverter.h
 *
 *  Created on: Feb 2, 2015
 *      Author: salwatorska
 */

#ifndef URLPARAMCONVERTER_H_
#define URLPARAMCONVERTER_H_

class UrlParamConverter {
public:
	UrlParamConverter();
	virtual ~UrlParamConverter();
	static std::string decode(const std::string & sSrc);
	static std::string encode(const std::string & sSrc);
private:
	const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
};

#endif /* URLPARAMCONVERTER_H_ */
