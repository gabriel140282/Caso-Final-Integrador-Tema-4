//
//  JSONParser.h
//  JSONLib
//
//  Created by Jorge Omar Medra Torres on 25/02/18.
//  Copyright © 2018 Jorge Omar Medra Torres. All rights reserved.
//

#ifndef JSONParser_h
#define JSONParser_h

/*
 #include<cstdlib>
 #include<iostream>
 #include<cstring>
 #include<iostream>
 #include<sstream>
 #include<iomanip>
 #include<utility>


 #include<string>
 #include<cstdio>
 #include<vector>
 #include<map>
 */

#include<cstdlib>
#include<iostream>


#include "JSONLib.h"

namespace json
{

    namespace utf8
    {
        class JSONParser
        {
            std::streambuf* _stream;
            int _offset;

            char getCurrentChar();
            void moveNextChar();
            void movePrevChar();
            void moveOffsetTo(int pos);
            //void findBeginToken();
            void skipEmptyChars();
            std::string readToken();
            void resetStreamOffset(int offset=0);
            std::tuple<JSONType, int, std::string> detectType();

            std::tuple<std::string, int, std::string> parseKey();
            std::tuple<JSONObject, int, std::string> parseValue();

            std::pair<int, std::string> parseObject(JSONObject& json);
            std::pair<int, std::string> parseArray(JSONObject& json);

            std::tuple<std::string, int, std::string> parseNull();
            std::tuple<bool, int, std::string>  parseBoolean();
            std::tuple<double, int, std::string> parseNumber();
            std::tuple<std::string, int, std::string> parseString();

        public:

            JSONParser():_offset{0}{}

            std::pair<int, std::string> parse(std::streambuf* stream, JSONObject&);
        };
    }

    namespace wide
    {
        class JSONParser
        {
            std::wstreambuf* _stream;
            int _offset;

            wchar_t getCurrentChar();
            void moveNextChar();
            void movePrevChar();
            void moveOffsetTo(int pos);
            //void findBeginToken();
            void skipEmptyChars();
            std::wstring readToken();
            void resetStreamOffset(int offset=0);
            std::tuple<JSONType, int, std::wstring> detectType();

            std::tuple<std::wstring, int, std::wstring> parseKey();
            std::tuple<JSONObject, int, std::wstring> parseValue();

            std::pair<int, std::wstring> parseObject(JSONObject& json);
            std::pair<int, std::wstring> parseArray(JSONObject& json);

            std::tuple<std::wstring, int, std::wstring> parseNull();
            std::tuple<bool, int, std::wstring>  parseBoolean();
            std::tuple<double, int, std::wstring> parseNumber();
            std::tuple<std::wstring, int, std::wstring> parseString();

        public:

            JSONParser():_offset{0}{}

            std::pair<int, std::wstring> parse(std::wstreambuf* stream, JSONObject&);
            std::wstring wconvert(std::string);
        };
    }
}


#endif /* JSONParser_h */
