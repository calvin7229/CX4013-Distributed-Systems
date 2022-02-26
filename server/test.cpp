#include "transform.h"

#include <iostream>

int main() {
    std::string x = "Hello World!";
    char bytes[x.size()];

    transform::marshalString(x, bytes);

    std::string y = transform::unmarshalString(bytes, x.size());
    // for (int i = 0; i < 4; i++) {
    //     printf("%d ", bytes[i]);
    // }

    std::cout << "x: " << x << ", y: " << y << std::endl;

    return 0;
}