#include<stdlib.h>
#include<iostream>
#include <vector>
#include <cstdarg>

#ifndef BIG_NUMBERS_BIG_NUMBER_HPP
#define BIG_NUMBERS_BIG_NUMBER_HPP
namespace BigNumber {
    const static char *symbols = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const static char *inf = "Infinity";

    const static int MAX_BASE = 37;

    typedef unsigned char digit;

    class BigNumberError : std::exception {
    };

    class base_difference : public BigNumberError {
    };

    class invalid_base : public BigNumberError {
    };

    class invalid_number : public BigNumberError {
    };

    class uncertainty : public BigNumberError {
    };

    unsigned int ToIndex(const BigNumber::digit &c_digit);

    class UInt;

    UInt operator+(const UInt &a, const UInt &b);

    UInt operator-(const UInt &a, const UInt &b);

    bool operator>=(const UInt &a, const UInt &b);

    bool operator!=(const UInt &a, const UInt &b);

    bool operator<=(const UInt &a, const UInt &b);

    bool operator>(const UInt &a, const UInt &b);
}

unsigned int BigNumber::ToIndex(const BigNumber::digit &c_digit) {
    unsigned int digit = c_digit;

    if (digit > 47 && digit < 58) {
        digit -= 48;
    } else if (digit > 64 && digit < 91) {
        digit -= 55;
    } else if (digit > 96 && digit < 123) {
        digit -= 87;
    } else {
        throw BigNumber::invalid_number();
    }

    return digit;
}


class BigNumber::UInt {
protected:
    typedef enum {
        OP_SUB, OP_ADD, OP_MUL, OP_DIV
    } OPERATOR;
    std::vector<BigNumber::digit> number;
    const unsigned int base;

private:
    void RemoveOpeningZeros() {
        auto it = this->number.rbegin();
        while (this->number.size() > 1 && *it == BigNumber::symbols[0]) {
            this->number.pop_back();
            it = this->number.rbegin();
        }
    }

    void ALU(const UInt& x, const UInt& y, OPERATOR){
        auto x_it = x.number.begin();
        auto y_it = y.number.begin();
        auto x_end = x.number.end();
        auto y_end = x.number.end();
        unsigned int a,b,c;
        unsigned int carry = 0;
        BigNumber::digit digit;

        while(x_it != x_end || y_it != y_end){
            a = x_it != x_end ? BigNumber::ToIndex(*x_it) : 0;
            b = y_it != y_end ? BigNumber::ToIndex(*y_it) : 0;
            switch(OPERATOR){
                case OP_MUL:
                    c = (carry+a)*b;
                case OP_ADD:
                    c = carry+a+b;
                case OP_ADD:
                case OP_MUL:
                    carry = c >= x.base ? c/x.base : 0;
                case OP_SUB:
                    c =  carry ? (a-1) : a;
                    c = c>=b ? c-b: ((x.base-carry)+c)-b;
                    carry = c>=b ? 0 : carry+1;
                case OP_MUL:
                case OP_ADD:
                case OP_SUB:
                    digit = BigNumber::symbols[c%x.base];
                case OP_MUL:
                    x.number.push_back(digit);
                    ++y_it;
                    x_it = y_it == y_end? x_end : x_it;
                    break;
                case OP_ADD:
                    if(x_it == x_end){
                        x.number.push_back(digit);
                        continue;
                    }
                case OP_ADD:
                case OP_SUB:
                    *x_it = digit;
                    y_it = y_it == y_end ? y_it : y_it+1;
                    x_it = x_it == x_end ? x_it : x_it+1;
                    break;
            }
        }
    }

public:
    explicit UInt() : number(), base(10) {
        number.push_back(BigNumber::symbols[0]);
    };

    UInt(const UInt &number) : number(), base(number.base) {
        for (auto digit: number.number) {
            this->number.push_back(digit);
        }
    }

    explicit UInt(const std::string &number, const unsigned int &base = 10) : number(), base(base) {
        if (base > BigNumber::MAX_BASE || base < 2) {
            throw BigNumber::invalid_base();
        }

        std::string prefixed_number = number;

        for (auto it = prefixed_number.rbegin(); it != prefixed_number.rend(); ++it) {
            const unsigned int index = ToIndex(*it);

            if (index >= base) {
                throw BigNumber::invalid_base();
            }

            this->number.push_back(BigNumber::symbols[index]);
        }

        this->RemoveOpeningZeros();
    };

    UInt(const unsigned long long &number) : UInt(std::to_string(number)) {};

    bool IsFinite() const {
        return !this->number.empty();
    }

    friend bool operator==(const BigNumber::UInt &a, const BigNumber::UInt &b) {
        if (a.base != b.base) {
            throw BigNumber::base_difference();
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

    friend bool operator<(const BigNumber::UInt &a, const BigNumber::UInt &b) {
        if (a.base != b.base) {
            throw BigNumber::base_difference();
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

    UInt &operator=(const BigNumber::UInt &a) {
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
    void operator+=(const UInt &number) {
        if (number.base != this->base) {
            throw BigNumber::base_difference();
        }

        if (this->number.empty()) {
            return;
        }

        if (number.number.empty()) {
            this->number.clear();
            return;
        }

        this->BasicOP(number, ADD);
    }

    // Operator --x
    UInt &operator--() {
        (*this) -= 1;
        return *this;
    }

    // Operator x--
    UInt operator--(int) {
        UInt old = UInt(*this);
        operator--();
        return old;
    }

    void operator-=(const UInt &number) {
        if (this->base != number.base) {
            throw BigNumber::base_difference();
        }

        if (number.number.empty()) {
            throw uncertainty();
        }

        if (*this < number) {
            this->number.clear();
            return;
        }

        this->BasicOP(number, SUB);
    }

    void operator<<(const UInt &number) {
        if (!number.IsFinite()) {
            this->number.clear();
            return;
        }

        while (number > 0) {
            this->number.insert(this->number.begin(), BigNumber::symbols[0]);
        }
    }

    void operator>>(const UInt &number) {
        if (!number.IsFinite()) {
            this->number.clear();
            this->number.push_back(BigNumber::symbols[0]);
            return;
        }

        while (number > 0) {
            this->number.insert(this->number.begin(), BigNumber::symbols[0]);
        }
    }

    void operator*=(const UInt &number) {
        if (this->base != number.base) {
            throw BigNumber::base_difference();
        }

        if (this->number.empty()) {
            return;
        }

        if (number.number.empty()) {
            this->number.clear();
            return;
        }

        BigNumber::digit digit;
        unsigned int carry = 0;
        unsigned int mul = 0;
        int count = 0;

        UInt copy = *this;

        this->number.clear();
        this->number.push_back(BigNumber::symbols[0]);

        for (auto x = number.number.begin(); x != number.number.end(); ++x) {
            int dec = count;

            for (auto y = copy.number.begin(); y != copy.number.end(); ++y) {
                mul = (*y) * (*x + carry);
                digit = BigNumber::symbols[(mul) % (this->base)];
                carry = (mul) / this->base;
                (*this) += 0;
            }
            dec++;

        }

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

BigNumber::UInt BigNumber::operator+(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    BigNumber::UInt result = BigNumber::UInt(a);
    result += b;
    return result;
}

BigNumber::UInt BigNumber::operator-(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    BigNumber::UInt result = BigNumber::UInt(a);
    result -= b;
    return result;
}

bool BigNumber::operator>=(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    return !(a < b);
}

bool BigNumber::operator!=(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    return !(a == b);
}

bool BigNumber::operator<=(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    return (a == b || a < b);
}

bool BigNumber::operator>(const BigNumber::UInt &a, const BigNumber::UInt &b) {
    return !(a >= b);
}

#endif //BIG_NUMBERS_BIG_NUMBER_HPP
