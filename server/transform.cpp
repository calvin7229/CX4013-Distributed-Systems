#include "transform.h"

// Function to transform and place resultant integer value into a 4-byte array, big-endian style
void transform::marshalInt(int input, char* bytes) {
    int index = 0;

    for (int shift = 24; shift >= 0; shift -= 8) {
        *(bytes + index) = input >> shift;
        index++;
    }
}

// Function to transform and place resultant float value into a 4-byte array, big-endian style
void transform::marshalFloat(float input, char* bytes) {
    int integerRep;

    /*
        While integer and floating-point values are both 4-bytes represented, they have different internal representations.
        Using "memcpy", which is essentially a bitwise copy function, we can transfer the representation of input into an 
        integer variable and reuse the "marshalInt" function that we have just implemented to marshall floating-point values.
        (Since "marshalInt" is also just some bit shift operations, this make unmarshalling easier later)
    */
    memcpy(&integerRep, &input, 4);
    transform::marshalInt(integerRep, bytes);
}

// Function to transform and place resultant string value into a string.size()-byte array, big-endian style
void transform::marshalString(std::string input, char* bytes) {
    int index = 0;

    for (int i = 0; i < input.size(); i++) {
        *(bytes + index) = input[i];
        index++;
    }
}

