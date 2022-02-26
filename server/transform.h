#pragma once

#include <string>
#include <cstring>

/* 
    All of the data transformation functions below are based on the big-endian byte order
    Big-endian:     MSB is stored at the lowest storage address
    Little-endian:  LSB is stored at the lowest storage address
*/
namespace transform {
    void marshalInt(int input, char* bytes);
    void marshalFloat(float input, char* bytes);
    void marshalString(std::string input, char* bytes);

    int unmarshalInt(char* bytes);
    float unmarshalFloat(char* bytes);
    std::string unmarshalString(char* bytes);
}