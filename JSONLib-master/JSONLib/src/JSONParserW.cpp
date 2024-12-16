//
//  JSONParser.cpp
//  JSONLib
//
//  Created by Jorge Omar Medra Torres on 02/04/18.
//  Copyright © 2018 Jorge Omar Medra Torres. All rights reserved.
//

#include "JSONLib.h"
#include "JSONParser.h"
#include "JSONUnicode.h"
#include <stdio.h>
#include <iostream>

using namespace std;
using namespace json::wide;
using namespace json::common;

std::wstring JSONParser::wconvert(std::string str)
{
    wstring result;
    result.assign(str.begin(), str.end());
    return result;
}

wchar_t JSONParser::getCurrentChar()
{
    if(!_stream) return static_cast<wchar_t>(0);
    wchar_t c  = _stream->sgetc();
    return c != WEOF ? c : (wchar_t)0x00;
}

void JSONParser::moveNextChar()
{
    if(!_stream && getCurrentChar() == '\0') return;
    wchar_t c = _stream->snextc();
    if(c != WEOF)_offset++;
}

void JSONParser::movePrevChar()
{
    if(!_stream && _offset > 0) return;
    moveOffsetTo(_offset-1);
}
void JSONParser::moveOffsetTo(int pos)
{
    if(_stream)
    {
        _stream->pubseekpos(pos);
        _offset=pos;
    }
}
/*
 void JSONParser::findBeginToken()
 {
 char c = '\0';
 //int cOffset = _offset;
 while((c=getCurrentChar()))
 {
 if(c!= ' ' && c!= ',' && c!= '}' && c!= ']' && c!= '\n') break;
 moveNextChar();
 }

 }
 */
void JSONParser::skipEmptyChars()
{
    wchar_t c = '\0';
    while((c=getCurrentChar()))
    {
        if(c!= L' ' && c!= L'\n') break;
        moveNextChar();
    }
}

wstring JSONParser::readToken()
{
    wstringstream out;
    wchar_t c = '\0';

    skipEmptyChars();
    while((c=getCurrentChar()))
    {
        if(c!= ' ' && c!= ',' && c!= '}' && c!= '\n')
            out << c;
        else
            break;
        moveNextChar();
    }
    return out.str();
}

void JSONParser::resetStreamOffset(int offset)
{
    if(_stream)
    {
        _stream->pubseekpos(offset);
        _offset=offset;
    }
}

/************************
 Input:

 jsondoc: JSON Document
 json:    Reference to JSONData whic will be stored the parse result.

 return:

 <0> ErrCode:int
 <1> ErrDesc:string
 ************************/
std::pair<int,wstring> JSONParser::parse(wstreambuf* jsondoc, JSONObject& json)
{
    std::wostringstream outErr;

    _stream = jsondoc;
    resetStreamOffset(0);

    pair<int, wstring> result = parseObject(json);

    if(result.first == ERR_C_EXP_OBJARR)
    {
        resetStreamOffset(0);
        result = parseArray(json);
    }

    if(result.first != ERR_C_OK)
    {
        //json.clean();
        outErr << U"Error at position[" << _offset <<"]: "<< result.second;
        result.second = outErr.str();
        return result;
    }

    return std::make_pair(ERR_C_OK, wconvert(ERR_D_OK));
}

/************************

 return:

 <0> Ker:string
 <1> ErrCode:int
 <2> ErrDesc:string
 ************************/
std::tuple<wstring, int, wstring> JSONParser::parseKey()
{
    return parseString();
}


/************************

 return:

 <0> Ker:string
 <1> ErrCode:int
 <2> ErrDesc:string
 ************************/
std::tuple<wstring, int, wstring> JSONParser::parseString()
{
    std::wostringstream out;
    wchar_t c = '\0';
    bool bStarted = true;
    bool bSlash = false;
    bool bFormatOk = true;
    bool keepParsing = true;

    skipEmptyChars();
    while(keepParsing && (c=getCurrentChar()))
    {

        if(bStarted)
        {
            if(c != '\"')
            {
                bFormatOk = false;
                keepParsing = false;;
            }
            else bStarted = false;
        }
        else //bStarted
        {
            if(c == '\\') bSlash = true; //turn on Slash Band
            else
            {
                if(!bSlash && c == '\"') keepParsing = false;//End of String.
                if(bSlash) bSlash = false; //turn off Slash Band
            }

            if(keepParsing) out << c;
        }
        moveNextChar();
    }//while

    if(!bFormatOk)
        return std::make_tuple(L"", ERR_C_EXP_DLBQUOTE, wconvert(ERR_D_EXP_DLBQUOTE));

    JSONUnicode decode;
    wstring strDecode = decode.decodeString(out.str());

    return std::make_tuple(strDecode, ERR_C_OK, wconvert(ERR_D_OK));
}

std::tuple<wstring, int, wstring> JSONParser::parseNull()
{
    std::wostringstream out;

    out << readToken();
    wstring nl = out.str();

    if(wcscmp(nl.c_str(),L"null") != 0)
        return std::make_tuple(L"", ERR_C_EXP_NULL, wconvert(ERR_D_EXP_NULL));

    return std::make_tuple(nl, ERR_C_OK, wconvert(ERR_D_OK));
}

std::tuple<bool, int, wstring>  JSONParser::parseBoolean()
{
    std::wostringstream out;

    //int _curPos = _offset;

    out << readToken();
    wstring nl = out.str();

    if(wcscmp(nl.c_str(),L"true") == 0)
        return std::make_tuple(true, ERR_C_OK, wconvert(ERR_D_OK));
    else if(wcscmp(nl.c_str(),L"false") == 0)
        return std::make_tuple(true, ERR_C_OK, wconvert(ERR_D_OK));

    //_offset = _curPos+1;
    return std::make_tuple(false, ERR_C_EXP_BOOL, wconvert(ERR_D_EXP_BOOL));
}

std::tuple<double, int, wstring> JSONParser::parseNumber()
{
    wchar_t c = '\0';
    double val = 0;
    bool bStarted = true;
    bool bExp = false;
    std::tuple<double,int,wstring> result;
    std::wostringstream out;


    skipEmptyChars();
    while((c=getCurrentChar()))
    {
        if(!bStarted)
            if(c== ' ' || c== ',' || c== '}' || c== '\n')
                break;

        if(bStarted)
        {
            if( ( (int)c >= 48 && (int)c <= 57 ) || c=='-') //Digits and sign at the begin are allowed.
                bStarted = false;
            else
                return make_tuple(0,ERR_C_EXP_NUMBER, wconvert(ERR_D_EXP_NUMBER));
        }
        else
        {
            if( ( (int)c >= 48 && (int)c <= 57 ) || //Digits
               (!bExp && c=='.') ||                //Dot but not Exponent
               (bExp && (c=='+' || c=='-') ) )     //Exponent and sign
                bExp = false;
            else if(c=='E' || c=='e')                //Exponent
                bExp = true;
            else
                return make_tuple(0,ERR_C_EXP_NUMBER, wconvert(ERR_D_EXP_NUMBER));

        }
        out << c;
        moveNextChar();
    }//while

    wchar_t* pEnd;
    val = std::stod(out.str());// strtod(out.str().c_str() ,&pEnd);

    return make_tuple(val, ERR_C_OK, wconvert(ERR_D_OK));
}


/************************
 return:
 <0> JSONType
 <1> ErrCode:int
 <2> ErrDesc:string
 ************************/
std::tuple<JSONType, int, wstring> JSONParser::detectType()
{
    wchar_t c = '\0';
    JSONType type;
    bool bError = true;

    skipEmptyChars();
    while((c=getCurrentChar()))
    {
        if(c == 'n') // null value
        {
            type = JSONType::EMPTY;
            bError = false;
            break;
        }
        else if(c == 't' || c == 'f') // true or false
        {
            type = JSONType::Boolean;
            bError = false;
            break;
        }
        else if( ((int)c >= 48 && (int)c <= 57) || c=='-' ) //Start with number or minus.
        {
            type = JSONType::Number;
            bError = false;
            break;
        }
        else if(c == '\"') // String
        {
            type = JSONType::String;
            bError = false;
            break;
        }
        else if(c == '{') // Object
        {
            type = JSONType::Object;
            bError = false;
            break;
        }
        else if(c == '[') // Object
        {
            type = JSONType::Array;
            bError = false;
            break;
        }
    }//while((c=getCurrentChar()))


    if(bError)
        return make_tuple(type, ERR_C_EXP_OBJARR, wconvert(ERR_D_EXP_OBJARR));

    return make_tuple(type, ERR_C_OK, wconvert(ERR_D_OK));
}

/************************
 return:
 <0> JSONObject: The KVP's value.
 <1> ErrCode:int
 <2> ErrDesc:string
 ************************/
std::tuple<JSONObject, int, wstring> JSONParser::parseValue()
{
    JSONObject value; //By Default it value is EMPTY (NULL)
    JSONType type;

    skipEmptyChars();
    auto dType = detectType();
    type = std::get<0>(dType);

    if(std::get<1>(dType) != ERR_C_OK)
        return make_tuple(value, ERR_C_EXP_OBJARR, wconvert(ERR_D_EXP_OBJARR));

    switch (type) {
        case JSONType::String:
        {
            auto sVal = parseString();
            value = std::get<0>(sVal);
            return make_tuple(value, std::get<1>(sVal), std::get<2>(sVal));
        }
        case JSONType::Boolean:
        {
            auto bVal = parseBoolean();
            value = std::get<0>(bVal);
            return make_tuple(value, std::get<1>(bVal), std::get<2>(bVal));
        }
        case JSONType::Number:
        {
            auto nVal = parseNumber();
            value = std::get<0>(nVal);
            return make_tuple(value, std::get<1>(nVal), std::get<2>(nVal));
        }
        case JSONType::EMPTY:
        {
            auto nlVal = parseNull(); //Check if it is an Valid Null Value.
            return make_tuple(value, std::get<1>(nlVal), std::get<2>(nlVal));
        }
        case JSONType::Object:
        {
            auto oVal = parseObject(value); //it returns an std::pair<int,string>
            return make_tuple(value, oVal.first, oVal.second);
        }
        case JSONType::Array:
        {
            auto oVal = parseArray(value); //it returns an std::pair<int,string>
            return make_tuple(value, oVal.first, oVal.second);
        }
    }
    return make_tuple(value, ERR_C_EXP_OBJARR, wconvert(ERR_D_EXP_OBJARR));
}



/************************
 return:

 <0> ErrCode:int
 <1> ErrDesc:string
 ************************/
std::pair<int,wstring> JSONParser::parseObject(JSONObject& json)
{
    wchar_t c = '\0';
    wstring key_;
    bool bStarted = true;
    bool readValue = false;
    bool bkeepParsing = true;

    skipEmptyChars();
    while(bkeepParsing && (c=getCurrentChar()))
    {
        if(bStarted && (c == '{' || c == ',')) //Getting the Key
        {
            if(bStarted) bStarted = false;
            moveNextChar(); //avoid '{' or ','

            auto result = parseKey();

            if(std::get<1>(result) != ERR_C_OK)
                return std::make_pair(std::get<1>(result),std::get<2>(result));
            else
                key_ = std::get<0>(result);

            skipEmptyChars();
            if(getCurrentChar() != ':') return std::make_pair(ERR_C_EXP_COLON, wconvert(ERR_D_EXP_COLON));
            readValue = true;
        }
        else if(readValue) //Getting Value
        {
            readValue = false;
            auto val = parseValue();
            JSONObject value;
            value = std::get<0>(val);

            if(std::get<1>(val) != ERR_C_OK)
            {
                value.clean();
                return std::make_pair(std::get<1>(val), std::get<2>(val));
            }

            //cout << endl << "[" << name_ << "]" << value.toString() << endl;
            value.keyVal(key_);
            json += value; //Add the value to the JSONObject, using operator '+='
            key_ = L"";

            skipEmptyChars();
            if(getCurrentChar() == ',')
            {
                bStarted = true; //New KVP
                movePrevChar();
            }
            else if(getCurrentChar() == '}')
                bkeepParsing = false; //End of the object
        }
        moveNextChar();
    }//while


    if(bStarted)
        return std::make_pair(ERR_C_EXP_OBJARR, wconvert(ERR_D_EXP_OBJARR));

    return std::make_pair(ERR_C_OK, wconvert(ERR_D_OK));
}


/************************
 return:

 <0> ErrCode:int
 <1> ErrDesc:string
 ************************/
std::pair<int,wstring> JSONParser::parseArray(JSONObject& json)
{
    wchar_t c = '\0';
    bool bStarted = true;
    bool bLookForValue = true;
    bool bkeepParsing = true;

    skipEmptyChars();
    while(bkeepParsing && (c=getCurrentChar()))
    {
        if(bStarted && c == '[') //It must start with '['
        {
            bStarted = false;
            bLookForValue = true;
        }
        else if(!bStarted && bLookForValue)
        {
            bLookForValue = false;
            std::tuple<JSONObject, int, wstring> rest = parseValue();
            if(std::get<1>(rest) != ERR_C_OK) return make_pair(std::get<1>(rest), std::get<2>(rest));
            json += std::get<0>(rest);

            skipEmptyChars();
            if(getCurrentChar() == ',')bLookForValue = true; //New Value
            else if(getCurrentChar() == ']') bkeepParsing = false; //end of array.
        }
        moveNextChar();
    }

    if(bStarted)
        return std::make_pair(ERR_C_EXP_ARR, wconvert(ERR_D_EXP_ARR));

    return std::make_pair(ERR_C_OK, wconvert(ERR_D_OK));
}

