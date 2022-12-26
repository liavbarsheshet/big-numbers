#include<stdlib.h>
#include<iostream>
#include<vector>
#include "big-number.hpp"

#ifndef BIG_NUMBERS_INT_HPP
#define BIG_NUMBERS_INT_HPP
namespace BigNumber {
    class UInt;
}

class BigNumber::UInt {
    std::vector<char> number;
    unsigned int base;

    bool IsValidDigit(const unsigned char &digit) {
        const unsigned int dec_digit = digit;
        unsigned int index;
        if (digit > 47 && digit < 58) {
            index = digit - 48;
        } else if (digit > 64 && digit < 91) {
            index = digit - 55;
        } else if (digit > 96 && digit < 123) {
            index = digit - 87;
        }
        // TODO: Change Type Error
        throw BigNumber::Error("Invalid Number");
    }

public:
    explicit UInt(const unsigned int &base = 10) : number(), base(base) {};

    UInt(const std::string &number, const unsigned int &base = 10) : number(), base(base) {

    };
};

#endif
