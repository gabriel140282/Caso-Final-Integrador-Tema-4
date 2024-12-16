cmake_minimum_required(VERSION 3.29)
project(Caso_Final_Integrador_Tema_4)

set(CMAKE_CXX_STANDARD 14)

include_directories(json11-master)

add_executable(Caso_Final_Integrador_Tema_4
    json11-master/json11.cpp
    json11-master/json11.h
    main.cpp
        JSONLib-master/JSONLib/src/JSONLib.cpp
        JSONLib-master/JSONLib/src/JSONLibW.cpp
        JSONLib-master/JSONLib/src/JSONParser.cpp
        JSONLib-master/JSONLib/src/JSONParserW.cpp
        JSONLib-master/JSONLib/src/JSONUnicode.cpp)
