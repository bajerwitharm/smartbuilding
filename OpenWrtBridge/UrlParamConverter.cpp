/*
 * UrlParamConverter.cpp
 *
 *  Created on: Feb 2, 2015
 *      Author: salwatorska
 */

#include "UrlParamConverter.h"

UrlParamConverter::UrlParamConverter() {
	// TODO Auto-generated constructor stub

}

UrlParamConverter::~UrlParamConverter() {
	// TODO Auto-generated destructor stub
}

std::string UrlParamConverter::decode(const std::string & sSrc)
{
   // Note from RFC1630: "Sequences which start with a percent
   // sign but are not followed by two hexadecimal characters
   // (0-9, A-F) are reserved for future extension"

   const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
   const int SRC_LEN = sSrc.length();
   const unsigned char * const SRC_END = pSrc + SRC_LEN;
   // last decodable '%'
   const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

   char * const pStart = new char[SRC_LEN];
   char * pEnd = pStart;

   while (pSrc < SRC_LAST_DEC)
   {
      if (*pSrc == '%')
      {
         char dec1, dec2;
         if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
            && -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
         {
            *pEnd++ = (dec1 << 4) + dec2;
            pSrc += 3;
            continue;
         }
      }

      *pEnd++ = *pSrc++;
   }

   // the last 2- chars
   while (pSrc < SRC_END)
      *pEnd++ = *pSrc++;

   std::string sResult(pStart, pEnd);
   delete [] pStart;
   return sResult;
}


std::string UrlParamConverter::encode(const std::string & sSrc)
{
   const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
   const int SRC_LEN = sSrc.length();
   unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
   unsigned char * pEnd = pStart;
   const unsigned char * const SRC_END = pSrc + SRC_LEN;

   for (; pSrc < SRC_END; ++pSrc)
   {
      if (SAFE[*pSrc])
         *pEnd++ = *pSrc;
      else
      {
         // escape this char
         *pEnd++ = '%';
         *pEnd++ = DEC2HEX[*pSrc >> 4];
         *pEnd++ = DEC2HEX[*pSrc & 0x0F];
      }
   }

   std::string sResult((char *)pStart, (char *)pEnd);
   delete [] pStart;
   return sResult;
}

