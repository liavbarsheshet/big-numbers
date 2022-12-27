#include<stdlib.h>
#include<iostream>
#include "big-number.hpp"

int main() {
    try {
        BigNumber::UInt num = "1";
        std::cout << num;

    }
    catch (const std::exception &err) {
        std::cerr << err.what();
    }
    return 0;
}
