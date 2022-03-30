#pragma once

#include <string>
#include <cstring>
#include <iostream>

/* 
    All of the data transformation functions below are based on the big-endian byte order
    Big-endian:     MSB is stored at the lowest storage address
    Little-endian:  LSB is stored at the lowest storage address
*/
namespace transform {
    void marshalChar(char input, char* bytes);
    void marshalInt(int input, char* bytes);
    void marshalFloat(float input, char* bytes);
    void marshalString(std::string input, char* bytes);

    char unmarshalChar(char* bytes);
    int unmarshalInt(char* bytes);
    float unmarshalFloat(char* bytes);
    std::string unmarshalString(char* bytes, int length);
}