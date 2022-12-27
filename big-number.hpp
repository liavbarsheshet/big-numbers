#include<stdlib.h>
#include<iostream>
#include <vector>
#include <cstdarg>

#ifndef BIG_NUMBERS_BIG_NUMBER_HPP
#define BIG_NUMBERS_BIG_NUMBER_HPP
namespace BigNumber {
    const static char *symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const static char *inf = "∞";

    const static int MAX_BASE = 37;

    typedef unsigned char digit;

    class Error;

    unsigned int ToIndex(const BigNumber::digit &c_digit);

    class UInt;
}


class BigNumber::Error : public std::exception {
    std::string message;
public:
    Error() : std::exception() {};

    Error(const std::string &message) : message(message) {}

    virtual const char *what() const noexcept override {
        return this->message.c_str();
    }
};

unsigned int BigNumber::ToIndex(const BigNumber::digit &c_digit) {
    unsigned int digit = c_digit;

    if (digit > 47 && digit < 58) {
        digit -= 48;
    } else if (digit > 64 && digit < 91) {
        digit -= 55;
    } else if (digit > 96 && digit < 123) {
        digit -= 87;
    } else {
        throw BigNumber::Error("Invalid number.");
    }

    return digit;
}


class BigNumber::UInt {
protected:
    std::vector<BigNumber::digit> number;
    const unsigned int base;

public:
    explicit UInt() : number(), base(10) {
        number.push_back('0');
    };

    UInt(const UInt &a) : number(), base(a.base) {
        for (auto a: a.number) {
            this->number.push_back(a);
        }
    }

    explicit UInt(const std::string &number, const unsigned int &base = 10) : number(), base(base) {
        if (base > BigNumber::MAX_BASE || base < 2) {
            throw BigNumber::Error("Invalid base.");
        }

        // Removes opening zeros
        std::string prefixed_number = number;
        while (prefixed_number[0] == '0' && prefixed_number.length() > 1) {
            prefixed_number = prefixed_number.substr(1);
        }

        for (auto it = prefixed_number.rbegin(); it != prefixed_number.rend(); ++it) {
            const unsigned int index = ToIndex(*it);

            if (index >= base) {
                throw BigNumber::Error("Invalid number.");
            }

            this->number.push_back(BigNumber::symbols[index]);
        }
    };

    UInt(const unsigned long long &number) : UInt(std::to_string(number)) {};

    friend bool operator==(const UInt &a, const UInt &b) {
        if (a.base != b.base) {
            throw BigNumber::Error("Two numbers differs by base.");
        }

        if (a.number.size() != b.number.size()) {
            return false;
        }

        auto a_it = a.number.begin();
        auto b_it = b.number.begin();


        while (a_it != a.number.end() && a_it != a.number.end()) {
            if (*a_it != *b_it) {
                return false;
            }

            ++a_it;
            ++b_it;
        }

        return true;
    }

    friend bool operator<(const UInt &a, const UInt &b) {
        if (a.base != b.base) {
            throw BigNumber::Error("Two numbers differs by base.");
        }

        if (a.number.empty() && b.number.empty()) {
            return false;
        }

        if (b.number.empty()) {
            return true;
        }
        if (a.number.size() != b.number.size()) {
            return a.number.size() < b.number.size();
        }

        auto a_it = a.number.rbegin();
        auto b_it = b.number.rbegin();

        for (; a_it != a.number.rend(); ++a_it, ++b_it) {
            if ((*a_it) == (*b_it)) {
                continue;
            }
            return (*a_it) < (*b_it);
        }

        return false;

    }

    UInt &operator=(const UInt &a) {
        this->number.clear();

        for (auto &digit: a.number) {
            this->number.push_back(digit);
        }

        return *this;
    }

    // Operator ++x
    UInt &operator++() {
        (*this) += 1;
        return *this;
    }

    // Operator x++
    UInt operator++(int) {
        UInt old = UInt(*this);
        operator++();
        return old;
    }

    // Operator +
    void operator+=(const UInt &a) {
        if (a.base != this->base) {
            throw BigNumber::Error("Two numbers differs by base.");
        }

        if (this->number.empty()) {
            return;
        }

        if (a.number.empty()) {
            this->number.clear();
            return;
        }

        unsigned int carry = 0;

        auto a_it = a.number.begin();
        auto a_end = a.number.end();
        auto b_it = this->number.begin();
        auto b_end = this->number.end();

        while (b_it != b_end || carry) {
            unsigned int a_index = (a_it == a.number.end() ? 0 : BigNumber::ToIndex(*a_it));
            unsigned int b_index = (b_it == this->number.end() ? 0 : BigNumber::ToIndex(*b_it));

            unsigned int sum = a_index + b_index + carry;
            BigNumber::digit digit = BigNumber::symbols[(sum) % (base)];
            carry = (sum) / base;

            if (a_it != a_end) {
                ++a_it;
            }

            if (b_it != this->number.end()) {
                *b_it = digit;
                ++b_it;
            } else {
                this->number.push_back(digit);
            }
        }
    }

    void operator-=(const UInt &a) {

    }

    friend std::ostream &operator<<(std::ostream &os, const UInt &n) {
        if (n.number.empty()) {
            os << BigNumber::inf;
            return os;
        }
        for (auto it = n.number.rbegin(); it != n.number.rend(); ++it) {
            os << *it;
        }

        return os;
    }
};

BigNumber::UInt operator+(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    BigNumber::UInt result = BigNumber::UInt(a);
    result += b;
    return result;
}

bool operator>=(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    return !(a < b);
}

bool operator!=(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    return !(a == b);
}

bool operator<=(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    return (a == b || a < b);
}

bool operator>(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    return !(a <= b);
}

#endif //BIG_NUMBERS_BIG_NUMBER_HPP
