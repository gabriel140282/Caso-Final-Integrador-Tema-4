//
//  JSONLib.h
//  JSONLib
//
//  Created by Jorge Omar Medra Torres on 13/02/18.
//  Copyright © 2018 Jorge Omar Medra Torres. All rights reserved.
//



#ifndef JSONLIB_H_
#define JSONLIB_H_

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


namespace json
{
    namespace common
    {
        const std::string VERSION = "JSONLib V2.0";
        //enum class JSONType:size_t {EMPTY = 0, String, Number, Boolean, Object, Array};
        enum class JSONType:size_t {EMPTY = 0, String=1, Number=2, Boolean=3, Object=4, Array=5};

        const uint   ERR_C_OK           = 0;
        const std::string ERR_D_OK           = "OK\0";
        const uint   ERR_C_EXP_OBJARR   = 1;
        const std::string ERR_D_EXP_OBJARR 	= "Invalid message format, it expected an object '{}' or array '[]'\0";
        const uint   ERR_C_EXP_DLBQUOTE = 2;
        const std::string ERR_D_EXP_DLBQUOTE = "Invalid message format, it expected a double quote '\"'\0";
        const uint   ERR_C_EXP_NULL     = 3;
        const std::string ERR_D_EXP_NULL     = "Invalid message format, it expected a null value 'null'\0";
        const uint   ERR_C_EXP_BOOL     = 4;
        const std::string ERR_D_EXP_BOOL     = "Invalid message format, it expected a boolean value 'true' or 'false'\0";
        const uint   ERR_C_EXP_NUMBER   = 5;
        const std::string ERR_D_EXP_NUMBER   = "Invalid message format, it expected a number value (log|value)\0";
        const uint   ERR_C_EXP_ARR      = 6;
        const std::string ERR_D_EXP_ARR      = "Invalid message format, it expected an array '[]'\0";
        const uint   ERR_C_EXP_COLON    = 7;
        const std::string ERR_D_EXP_COLON    = "Invalid message format, it expected a colon char ':'\0";

        const uint   ERR_C_NULL_REF     = 100;
        const std::string ERR_D_NULL_REF     = "It's not allowed set an Null reference to JSONElement& object.\0";
        const uint   ERR_C_FILE_NF      = 101;
        const std::string ERR_D_FILE_NF      = "Fle Not Found.\0";
    }

    namespace utf8
    {
        using namespace json::common;

        class JSONObject
        {

            common::JSONType _type;
            std::string  _key;
            bool _bValue;
            bool _bInteger;
            int _lastIndexInKeys;
            std::string _lastKey;
            std::vector<std::pair<std::string,JSONObject>> _array;
            std::map<std::string, std::vector<int>> _keys;

            std::string  _strValue;
            bool    _boolValue;
            double  _numValue;

            void type(common::JSONType t);

            std::string printString(bool bJSONFormat=true);
            std::string printNumber();
            std::string printObject(bool asArray=false);

        public:

            JSONObject();
            ~JSONObject();

            std::pair<int,std::string>loadJSON(const std::string& json);
            std::pair<int,std::string>loadJSONByFile(const std::string path);

            void clean();
            common::JSONType type();

            void keyVal(std::string);
            std::string keyVal();

            std::string toString(bool bJSONFormat=true);

            void value(double);
            void value(bool b);
            void value(std::string s);

            JSONObject& operator=(int);
            JSONObject& operator=(long);
            JSONObject& operator=(float);
            JSONObject& operator=(double);
            JSONObject& operator=(bool);
            JSONObject& operator=(std::string);
            JSONObject& operator=(const char*);

            std::string strValue();
            double numValue();
            bool boolValue();

            JSONObject& operator[](int);
            JSONObject& operator[](std::string);
            JSONObject& operator+=(JSONObject&);

            size_t size();
        };
    }


    namespace wide
    {
        using namespace json::common;

        class JSONObject
        {

            JSONType _type;
            std::wstring  _key;
            bool _bValue;
            bool _bInteger;
            int _lastIndexInKeys;
            std::wstring _lastKey;
            std::vector<std::pair<std::wstring,JSONObject>> _array;
            std::map<std::wstring, std::vector<int>> _keys;

            std::wstring  _strValue;
            bool    _boolValue;
            double  _numValue;

            void type(JSONType t);

            std::wstring printString(bool bJSONFormat=true);
            std::wstring printNumber();
            std::wstring printObject(bool asArray=false);

        public:

            JSONObject();
            ~JSONObject();

            std::pair<int,std::wstring>loadJSON(const std::wstring& json);
            std::pair<int,std::wstring>loadJSONByFile(const std::string path);

            void clean();
            JSONType type();

            void keyVal(std::wstring);
            std::wstring keyVal();

            std::wstring toString(bool bJSONFormat=true);

            void value(double);
            void value(bool b);
            void value(std::wstring s);

            JSONObject& operator=(int);
            JSONObject& operator=(long);
            JSONObject& operator=(float);
            JSONObject& operator=(double);
            JSONObject& operator=(bool);
            JSONObject& operator=(std::wstring);
            JSONObject& operator=(const wchar_t*);

            std::wstring strValue();
            double numValue();
            bool boolValue();

            JSONObject& operator[](int);
            JSONObject& operator[](std::wstring);
            JSONObject& operator+=(JSONObject&);

            size_t size();
        };
    }



}

#endif /* JSONLIB_H_ */
