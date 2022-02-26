#include "transform.h"

#include <iostream>

int main() {
    int x = 1024;
    char bytes[4];

    transform::marshalInt(x, bytes);

    for (int i = 0; i < 4; i++) {
        printf("%d ", bytes[i]);
    }
    std::string s = "Hello";

    std::cout << sizeof(s[0]) << std::endl;

    return 0;
}