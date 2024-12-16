//
//  JSONUnicode.h
//  JSONLib
//
//  Created by Jorge Omar Medra Torres on 13/02/18.
//  Copyright © 2018 Jorge Omar Medra Torres. All rights reserved.
//

#ifndef JSONUnicode_h
#define JSONUnicode_h

#include<cstdlib>
#include<iostream>
#include<cstring>
#include<iostream>
#include<sstream>
#include<iomanip>
#include<string>

//using namespace std;

namespace json
{
    namespace utf8
    {
        class JSONUnicode
        {

        public:
            std::string encodeString(std::string);
            std::string decodeString(std::string);
        };
    }

    namespace wide
    {
        class JSONUnicode
        {

        public:
            std::wstring encodeString(std::wstring);
            std::wstring decodeString(std::wstring);
        };
    }
}

#endif /* JSONUnicode_h */
