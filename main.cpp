#include<stdlib.h>
#include<iostream>
#include "big-number.hpp"
#include<assert.h>

bool SumTest() {
    BigNumber::UInt num1, num2;
    for (long long i = 0; i < 50000; ++i) {
        try {
            assert((num1++) == i);
            assert((++num2) == i + 1);
            assert((num1 + num2) == (2 * (i + 1)));
        } catch (...) {
            return false;
        }
    }
    return true;
}

bool SubTest() {
    BigNumber::UInt num2 = 50000;
    unsigned int t = 50000;
    for (long long i = 50000; i <= 0; --i) {
        try {
            assert((num2 - i) == (t - i));
        } catch (...) {
            return false;
        }
    }
    return true;
}


int main() {
    std::cout << "[TEST] OPERATOR (+):\t" << (SumTest() ? "SUCCESS" : "FAILED") << std::endl;
    std::cout << "[TEST] OPERATOR (-):\t" << (SubTest() ? "SUCCESS" : "FAILED") << std::endl;
    return 0;
}
