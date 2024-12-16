/*
 * JSONUnicode.cpp
 *
 *  Created on: Aug 13, 2016
 *      Author: Jorge Omar Medra Torres
 *		WebSite: www.jorgemedra.net
 */

#include "JSONUnicode.h"

using namespace std;
namespace u = json::utf8;
namespace w = json::wide;

/**
 * Method: encode
 *
 * Method to encode a sting to string with UNICODE charateres using tow-characters scape.
 */
string u::JSONUnicode::encodeString(string str)
{
	int offset = 0;
	size_t size = 0;
	std::ostringstream out;

	size = str.size();
	if(size <= 0)
		return "";

	while( offset < size )
	{
		char c = str[offset];
		offset++;

		switch (c)
		{
			case '\u005c': // '\'
				out << '\u005c' << '\u005c';
				break;
			case '\u0022': // '"'
				out << '\u005c' << '\u0022';
				break;
			case '\u002f': // '/'
				out << '\u005c' << '\u002f';
				break;
			case '\u0008': // '\b'
				out << '\u005c' << 'b';
				break;
			case '\u000a': // '\n'
				out << '\u005c' << 'n';
				break;
			case '\u000d': // '\r'
				out << '\u005c' << 'r';
				break;
			case '\u0009': // '\t'
				out << '\u005c' << 't';
				break;
			case '\u000c': // '\f'
				out << '\u005c' << 'f';
				break;
			default:
				out << c;
				break;
		}//switch

	}//while

    return out.str();
}

string u::JSONUnicode::decodeString(string ucdStr)
{
	int offset = 0;
	std::ostringstream out;
	std::ostringstream outInt;

    if(ucdStr.size() <=0)
        return "";

    size_t size = ucdStr.size();


	while( offset < size )
	{
		string vUnicode;
		char c = ucdStr[offset];
		offset++;
		if(c == '\u005c')
		{
			c = ucdStr[offset];
			offset++;
			switch (c)
			{
				case 'b':
					c = '\u0008';
					break;
				case 'f':
					c = '\u000c';
					break;
				case 'n':
					c = '\u000a';
					break;
				case 'r':
					c = '\u000d';
					break;
				case 't':
					c = '\u0009';
					break;
				case 'u':	// DIGITOS '\u0023';
					//char cc = ucdStr[offset];
					for(int i=0; i<4;i++)
					{
						outInt << ucdStr[offset];
						offset++;

					}

					string vl = outInt.str();
					unsigned long v = std::strtoul(outInt.str().c_str(),NULL,16);
					c = (char)v;
					break;

			}//switch
		}

		out << c;

	}//while

	return out.str();
}


/**
 * Method: encode
 *
 * Method to encode a sting to string with UNICODE charateres using tow-characters scape.
 */
wstring w::JSONUnicode::encodeString(wstring str)
{
    int offset = 0;
    size_t size = 0;
    std::wostringstream out;

    size = str.size();
    if(size <= 0)
        return L"";

    while( offset < size )
    {
        char c = str[offset];
        offset++;

        switch (c)
        {
            case '\u005c': // '\'
                out << '\u005c' << '\u005c';
                break;
            case '\u0022': // '"'
                out << '\u005c' << '\u0022';
                break;
            case '\u002f': // '/'
                out << '\u005c' << '\u002f';
                break;
            case '\u0008': // '\b'
                out << '\u005c' << 'b';
                break;
            case '\u000a': // '\n'
                out << '\u005c' << 'n';
                break;
            case '\u000d': // '\r'
                out << '\u005c' << 'r';
                break;
            case '\u0009': // '\t'
                out << '\u005c' << 't';
                break;
            case '\u000c': // '\f'
                out << '\u005c' << 'f';
                break;
            default:
                out << c;
                break;
        }//switch

    }//while

    return out.str();
}

wstring w::JSONUnicode::decodeString(wstring ucdStr)
{
    int offset = 0;
    std::wostringstream out;
    std::wostringstream outInt;

    if(ucdStr.size() <=0)
        return L"";

    size_t size = ucdStr.size();


    while( offset < size )
    {
        string vUnicode;
        wchar_t c = ucdStr[offset];
        offset++;
        if(c == '\u005c')
        {
            c = ucdStr[offset];
            offset++;
            switch (c)
            {
                case 'b':
                    c = '\u0008';
                    break;
                case 'f':
                    c = '\u000c';
                    break;
                case 'n':
                    c = '\u000a';
                    break;
                case 'r':
                    c = '\u000d';
                    break;
                case 't':
                    c = '\u0009';
                    break;
                case 'u':	// DIGITOS '\u0023';
                    //char cc = ucdStr[offset];
                    for(int i=0; i<4;i++)
                    {
                        outInt << ucdStr[offset];
                        offset++;

                    }

                    unsigned long v = std::stol(outInt.str());//  std::str strtoul(outInt.str().c_str(),NULL,16);
                    c = (wchar_t)v;
                    break;

            }//switch
        }

        out << c;

    }//while

    return out.str();
}
