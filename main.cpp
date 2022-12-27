#include<stdlib.h>
#include<iostream>
#include "big-number.hpp"
#include<assert.h>

void SumTest() {
    std::cout << "[TEST] Operator + Testing" << std::endl;
    BigNumber::UInt num1, num2, num3;
    for (long long i = 0; i < 2999; ++i) {
        assert((num1++) == i);
        assert((++num2) == i + 1);
        assert((num1 + num2) == (2 * (i + 1)));
    }
}


int main() {
    SumTest();
    std::cout << "[TEST] SUM TEST: SUCCESS" << std::endl;
    return 0;
}
