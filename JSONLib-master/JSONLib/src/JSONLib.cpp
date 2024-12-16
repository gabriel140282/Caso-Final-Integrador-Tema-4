//
//  JSONLib.cpp
//  JSONLib
//
//  Created by Jorge Omar Medra Torres on 13/02/18.
//  Copyright © 2018 Jorge Omar Medra Torres. All rights reserved.
//

#include "JSONLib.h"
#include "JSONParser.h"
#include <math.h>
#include <fstream>

using namespace std;
using namespace json::utf8;
using namespace json::common;

JSONObject::JSONObject():
    _type{JSONType::EMPTY},
    _array{},
    _keys{},
    _key{""},
    _strValue{""},
    _boolValue{false},
    _numValue{0},
    _bValue{true},
    _bInteger{true},
    _lastIndexInKeys{-1},
    _lastKey{""}
{

}

JSONObject::~JSONObject()
{

}

void JSONObject::type(JSONType t)
{
    _strValue  = t == JSONType::String?_strValue:"";
    _boolValue = t == JSONType::Boolean?_boolValue:false;
    _numValue  = t == JSONType::Number?_numValue:0;
    _bInteger = false;
    _type = t;
}

size_t JSONObject::size()
{
    return _array.size();
}

std::pair<int,string> JSONObject::loadJSON(const string& json)
{
    stringstream buffer;
    buffer << json;

    unique_ptr<JSONParser> parser (new JSONParser);
    return parser->parse(buffer.rdbuf() ,*this);
}

std::pair<int,string> JSONObject::loadJSONByFile(const string path)
{
    std::ifstream istr(path, std::ifstream::in);
    unique_ptr<JSONParser> parser (new JSONParser);

    if (!istr)
        return std::make_pair(ERR_C_FILE_NF, ERR_D_FILE_NF);

    std::pair<int,string> res= parser->parse(istr.rdbuf() ,*this);
    istr.close();

    return res;
}



JSONType JSONObject::type()
{
    return _type;
}

void JSONObject::keyVal(string k)
{
    //if(_type == JSONType::Array) return;
    _bValue = k.compare("")?true:false;
    _key = k;
}

string JSONObject::keyVal()
{
    return _key;
}

void JSONObject::clean()
{
    type(JSONType::EMPTY);
    _array.clear();
    _keys.clear();
}


string JSONObject::printString(bool bJSONFormat)
{
    stringstream out;
    if(bJSONFormat)
        out << "\"" << _strValue << "\"";
    else
        out << _strValue;
    return out.str();
}

string JSONObject::printNumber()
{
    stringstream out;
    if(_bInteger)
        out << fixed << setprecision(0) << (long)_numValue;
    else
    {
        //out.setf(ios_base::floatfield);
        out << fixed << _numValue;

        //out.unsetf ( std::ios::floatfield );
    }
    return out.str();
}

/****************************************************************************************
 Each Object has values and each value is represented as a JObject.
 {
    "key1":value1,
    "key2":value2,
    "key3":value3,
 }
 ****************************************************************************************/
string JSONObject::printObject(bool asArray)
{
    stringstream out;
    out << (asArray?"[":"{");

    bool bi = false;
    //for(pair<string,JSONObject&> itm: _array)
    for(auto itm: _array)
    {
        if(bi) out << ",";
        else bi=true;
        out <<itm.second.toString();
    }
    out << (asArray?"]":"}");

    return out.str();
}



/************************************************************************
 The basic format to printm for Empty, Number, String and Bool is:

    "Key":Value

 If the object has an empty key the format is:

    Value

 Where Values has the proper format according with https://www.json.org/
************************************************************************/
string JSONObject::toString(bool bJSONFormat)
{
    stringstream out;
    string vl;

    switch (_type) {
        case JSONType::EMPTY:
            vl = "null";
            break;
        case JSONType::Boolean:
            vl = _boolValue?"true":"false";
            break;
        case JSONType::Number:
            vl = printNumber();
            break;
        case JSONType::String:
            vl = printString();
            break;
        case JSONType::Object:
            vl = printObject();
            break;
        case JSONType::Array:
            vl = printObject(true);
            break;
    }

    //if(!_bValue && _type != JSONType::Array)
    if(bJSONFormat && _key.compare("") != 0)
        out << "\"" << _key << "\":";
    out << vl;

    return out.str();
}

void JSONObject::value(double d)
{
    double intP, decP;

    type(JSONType::Number);
    decP = modf(d,&intP);
    _bInteger = (decP > 0)?false:true;
    this->_numValue = d;
}

void JSONObject::value(bool b)
{
    type(JSONType::Boolean);
    this->_boolValue = b;
}

void JSONObject::value(string s)
{
    type(JSONType::String);
    this->_strValue = s;
}

string JSONObject::strValue()
{
    return _strValue;
}

double JSONObject::numValue()
{
    return _numValue;
}

bool JSONObject::boolValue()
{
    return _boolValue;
}

/********************
 Operator Equal (=)
********************/

JSONObject& JSONObject::operator=(int i)
{
    value(static_cast<double>(i));
    return *this;
}

JSONObject& JSONObject::operator=(long l)
{
    value(static_cast<double>(l));
    return *this;
}

JSONObject& JSONObject::operator=(float f)
{
    value(static_cast<double>(f));
    return *this;
}

JSONObject& JSONObject::operator=(double d)
{
    value(d);
    return *this;
}

JSONObject& JSONObject::operator=(bool b)
{
    value(b);
    return *this;
}

JSONObject& JSONObject::operator=(string s)
{
    value(s);
    return *this;
}


JSONObject& JSONObject::operator=(const char* str)
{
    value(string(str));
    return *this;
}


JSONObject& JSONObject::operator[](int index)
{
    if(index < 0 || index >= _array.size())
        throw out_of_range("Index of item was out of bound.");

    return _array[index].second;
}

JSONObject& JSONObject::operator[](string key)
{
    int index = -1;
    map<string, vector<int>>::iterator it;
    it = _keys.find(key);

    if(it != _keys.end())
    {
        std::vector<int> idxs = it->second;
        _lastIndexInKeys = (key.compare(_lastKey) == 0) ? (_lastIndexInKeys + 1 < idxs.size() ? (_lastIndexInKeys + 1):0) : 0;
        index = idxs[_lastIndexInKeys];
    }
    else
    {
        string errstr;
        errstr.append("There is no Key [");
        errstr.append(key);
        errstr.append("] created into JSONObject.");
        throw invalid_argument(errstr);
    }

    _lastKey = key;
    return (*this)[index];
}

JSONObject& JSONObject::operator+=(JSONObject& other)
{
    int index = -1;
    string key = other.keyVal();
    map<string, vector<int>>::iterator it;

    it = _keys.find(key);
    bool bExist = it != _keys.end()? true : false;

    pair<string,JSONObject> nItem = std::make_pair(key,other);
    _array.push_back(nItem);
    index = static_cast<int>(_array.size() - 1);

    vector<int> vctr=  bExist? it->second : std::vector<int>{};
    vctr.push_back(index);

    if(!bExist)
        _keys.insert(std::make_pair(key, vctr));


    if(_keys.size()== 1 && _array.size() >1)
        type(JSONType::Array);
    else
        type(JSONType::Object);

    return (*this);
}





