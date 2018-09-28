//
// Created by ZV on 28.09.2018.
//

#ifndef BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_H
#define BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_H

#include <iostream>
#include <vector>
#include <regex>
#include <queue>

//TODO: separate this file on .h and .hpp

const std::size_t BIG_UNSIGNED_INT_SIZE = 64;

template<std::size_t N>
class BigUnsignedInt;

/**
 * Template of input operator for BigUnsignedInt of size N. Expects
 * a number in decimal system as input.
 * @tparam N - size of BigUnsignedInt to read
 * @param is - input stream from which BigUnsignedInt will be read
 * @param bigUnsignedInt - object to store number read
 * @return - reference to is
 */
template<std::size_t N>
std::istream& operator>>(std::istream& is, BigUnsignedInt<N>& bigUnsignedInt);

/**
 * Template of output operator for BigUnsignedInt of size N. Outputs
 * binary representation of provided object.
 * @tparam N - size of BigUnsignedInt to write
 * @param os - output stream
 * @param bigUnsignedInt - object to write to stream
 * @return - reference to os
 */
template<std::size_t N>
std::ostream& operator<<(std::ostream& os, const BigUnsignedInt<N>& bigUnsignedInt);

/**
 * Template of output operator for BigUnsignedInt rvalues of size N.
 * Outputs binary representation of provided object.
 * @tparam N - size of BigUnsignedInt to write
 * @param os - output stream
 * @param bigUnsignedInt - object to write to stream
 * @return - reference to os
 */
template<std::size_t N>
std::ostream& operator<<(std::ostream& os, BigUnsignedInt<N>&& bigUnsignedInt);

/**
 * Template of shift to the left operator for BigUnsignedInt of size N.
 * @tparam N - size of BigUnsignedInt
 * @param bigUnsignedInt - object to shift
 * @param shift - number of bits to shift
 * @return - copy of bigUnsignedInt shifted by the provided number of bits
 */
template<std::size_t N>
BigUnsignedInt<N> operator<<(BigUnsignedInt<N> bigUnsignedInt, std::size_t shift);

/**
 * Template of operator+ for BigUnsignedInt of size N.
 * @tparam N - size of BigUnsignedInt
 * @param a - first operand
 * @param b - second operand
 * @return - sum of first and second operands
 */
template<std::size_t N>
BigUnsignedInt<N> operator+(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b);

/**
 * Template of operator- for BigUnsignedInt of size N.
 * @tparam N - size of BigUnsignedInt
 * @param a - first operand
 * @param b - second operand
 * @return - difference between first and second operands
 */
template<std::size_t N>
BigUnsignedInt<N> operator-(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b);

/**
 * Tempalate of operator* for BigUnsignedInt of size N.
 * @tparam N - size of BigUnsignedInt
 * @param a - first operand
 * @param b - second operand
 * @return - product of first and second operands
 */
template<std::size_t N>
BigUnsignedInt<N> operator*(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b);

/**
 * BigUnsignedInt class represents unsigned integer which has N bits
 * in binary representation.
 * @tparam N - number of bits in unsigned integer
 */
template<std::size_t N = BIG_UNSIGNED_INT_SIZE>
class BigUnsignedInt {
public:

    using Unsigned = unsigned int;
    using UnsignedVector = std::vector<Unsigned>;
    using size_type = UnsignedVector::size_type;

    friend std::istream& operator>><N>(std::istream& is, BigUnsignedInt& bigUnsignedInt);

    friend std::ostream& operator<<<N>(std::ostream& os, const BigUnsignedInt& bigUnsignedInt);

    friend std::ostream& operator<<<N>(std::ostream& os, BigUnsignedInt&& bigUnsignedInt);

public:

    /// Default constructor
    BigUnsignedInt() : _binRepr(N) {}

    /**
     * Creates BigUnsignedInt object from string representation.
     * Throws same exceptions as createFromString for the same reasons.
     * @param stringRepr - string containing an unsigned number
     */
    explicit BigUnsignedInt(const std::string& stringRepr);

    /**
     * Increases this BigUnsignedInt by other.
     * @param other - BigUnsignedInt to add
     * @return - reference to *this object increased by other
     */
    BigUnsignedInt& operator+=(const BigUnsignedInt& other);

    /**
     * Decreases this BigUnsignedInt by other.
     * @param other - BigUnsignedInt to subtract
     * @return - reference to *this object decreased by other
     */
    BigUnsignedInt& operator-=(const BigUnsignedInt& other);

    /**
     * Multiplies this BigUnsignedInt by other.
     * @param other - BigUnsignedInt to multiply on
     * @return - reference to *this object multiplied on other
     */
    BigUnsignedInt& operator*=(const BigUnsignedInt& other);

    /**
     * Shifts this number to the left by provided number of bits.
     * @param shift - number of bits to shift by
     * @return - reference to *this object shifted by provided number of bits
     */
    BigUnsignedInt& operator<<=(std::size_t shift);

    /**
     * Checks if the result of last operation can be represented
     * as unsigned integer of size N.
     * @return - true, if result doesn't fit, false otherwise
     */
    static bool overflowed() { return _carryFlag > 0; }

private:
    /**
     * Stores binary representation of number contained in string in provided
     * object. Throws invalid argument exception if provided string contains something
     * else but digits, size of BigUnsignedInt is too small to represent provided number.
     * @param input - string containing an unsigned number
     * @param bigUnsignedInt - object to store the number converted from string
     */
    static void createFromString(const std::string& input, BigUnsignedInt& bigUnsignedInt);

private:
    UnsignedVector _binRepr;
    static Unsigned _carryFlag;
};

#include "BigUnsignedInt.hpp"

#endif //BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_H
