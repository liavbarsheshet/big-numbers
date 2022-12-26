#include<stdlib.h>
#include<iostream>
#include <utility>

#ifndef BIG_NUMBERS_BIG_NUMBER_HPP
#define BIG_NUMBERS_BIG_NUMBER_HPP
namespace BigNumber {
    const char *symbols = "0123456789abcdefghijklmnopqrstuvwxyz";

    const int MAX_BASE = 36;

    class Error;
}


class BigNumber::Error : public std::exception {
    std::string message;
public:
    Error() : std::exception() {};

    Error(const std::string &message) : message(message) {}

    // TODO: Override it
    const char *what() {
        return this->message.c_str();
    }
};

#endif //BIG_NUMBERS_BIG_NUMBER_HPP
