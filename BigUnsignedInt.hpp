//
// Created by ZV on 22.09.2018.
//

#ifndef BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_HPP
#define BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_HPP

#include <iostream>
#include <vector>
#include <regex>
#include <queue>

const std::size_t BIG_UNSIGNED_INT_SIZE = 64;

template<std::size_t N>
class BigUnsignedInt;

template<std::size_t N>
std::istream& operator>>(std::istream& is, BigUnsignedInt<N>& bigUnsignedInt);

template<std::size_t N>
std::ostream& operator<<(std::ostream& os, const BigUnsignedInt<N>& bigUnsignedInt);

template<std::size_t N>
std::ostream& operator<<(std::ostream& os, BigUnsignedInt<N>&& bigUnsignedInt);

template<std::size_t N>
BigUnsignedInt<N> operator<<(BigUnsignedInt<N> bigUnsignedInt, std::size_t shift);

template<std::size_t N>
BigUnsignedInt<N> operator+(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b);

template<std::size_t N>
BigUnsignedInt<N> operator-(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b);

template<std::size_t N>
BigUnsignedInt<N> operator*(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b);

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

    BigUnsignedInt() : _binRepr(N) {}

    explicit BigUnsignedInt(const std::string& stringRepr);

    BigUnsignedInt& operator+=(const BigUnsignedInt& other);

    BigUnsignedInt& operator-=(const BigUnsignedInt& other);

    BigUnsignedInt& operator*=(const BigUnsignedInt& other);

    BigUnsignedInt& operator<<=(std::size_t shift);

    static bool overflowed() { return _carryFlag > 0; }

private:

    static void createFromString(const std::string& input, BigUnsignedInt& bigUnsignedInt);

private:
    UnsignedVector _binRepr;
    static Unsigned _carryFlag;
};

template<size_t N>
typename BigUnsignedInt<N>::Unsigned BigUnsignedInt<N>::_carryFlag = 0;

template<std::size_t N>
std::istream& operator>>(std::istream& is, BigUnsignedInt<N>& bigUnsignedInt)
{
    using namespace std;
    // Check that provided input is unsigned number
    string input;
    is >> input;
    BigUnsignedInt<N>::createFromString(input, bigUnsignedInt);
    return is;
}

template<std::size_t N>
std::ostream& operator<<(std::ostream& os, const BigUnsignedInt<N>& bigUnsignedInt)
{
    typename BigUnsignedInt<N>::size_type counter = 1;
    for (const auto& bit:bigUnsignedInt._binRepr)
    {
        os << bit;
        if (counter++ % 4 == 0)
            os << " ";
    }
    return os;
}

template<std::size_t N>
std::ostream& operator<<(std::ostream& os, BigUnsignedInt<N>&& bigUnsignedInt)
{
    os << bigUnsignedInt;
    return os;
}

template<std::size_t N>
BigUnsignedInt<N> operator+(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b)
{
    BigUnsignedInt<N> result(a);
    return result += b;
}

template<std::size_t N>
BigUnsignedInt<N> operator-(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b)
{
    BigUnsignedInt<N> result(a);
    return result -= b;
}

template<std::size_t N>
BigUnsignedInt<N> operator*(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b)
{
    BigUnsignedInt<N> result(a);
    return result *= b;
}

template<std::size_t N>
BigUnsignedInt<N> operator<<(BigUnsignedInt<N> bigUnsignedInt, const size_t shift)
{
    return bigUnsignedInt <<= shift;
}

//TODO: try to make it with Boolean function
template<std::size_t N>
BigUnsignedInt<N>& BigUnsignedInt<N>::operator<<=(const std::size_t shift)
{
    BigUnsignedInt::_carryFlag = 0;

    // Shift completely erases number
    if (shift >= N)
    {
        for (Unsigned& bit:_binRepr)
            bit = 0;
        BigUnsignedInt::_carryFlag = 1;
        return *this;
    }

    // Find index of '1' appeared first from left in _binRepr
    std::size_t firstOneIndex;
    std::size_t index = 0;
    while (index < N - 1 && _binRepr[index] != 1)
        ++index;
    firstOneIndex = index;

    // Set the _carryFlag if we lose at least first '1' by applying shift
    if (shift > firstOneIndex)
        BigUnsignedInt<N>::_carryFlag = 1;

    // Shift the number and fill right part of it with '0'
    for (std::size_t i = shift; i < N; ++i)
        _binRepr[i - shift] = _binRepr[i];
    for (std::size_t i = N - shift; i < N; ++i)
        _binRepr[i] = 0;

    return *this;
}

template<std::size_t N>
BigUnsignedInt<N>& BigUnsignedInt<N>::operator+=(const BigUnsignedInt& other)
{
    BigUnsignedInt::_carryFlag = 0;
    for (typename BigUnsignedInt<N>::size_type i = N - 1;
         i != std::numeric_limits<typename BigUnsignedInt::size_type>::max(); --i)
    {
        Unsigned thisBit = this->_binRepr[i];
        _binRepr[i] = thisBit ^ other._binRepr[i] ^ BigUnsignedInt::_carryFlag;
        BigUnsignedInt::_carryFlag =
                (thisBit & other._binRepr[i]) | (BigUnsignedInt::_carryFlag & (thisBit ^ other._binRepr[i]));
    }
    return *this;
}

template<std::size_t N>
BigUnsignedInt<N>& BigUnsignedInt<N>::operator-=(const BigUnsignedInt& other)
{
    BigUnsignedInt::_carryFlag = 0;
    for (typename BigUnsignedInt::size_type i = N - 1;
         i != std::numeric_limits<typename BigUnsignedInt<N>::size_type>::max(); --i)
    {
        Unsigned thisBit = _binRepr[i];
        _binRepr[i] = thisBit ^ other._binRepr[i] ^ BigUnsignedInt::_carryFlag;
        BigUnsignedInt::_carryFlag =
                ((1 ^ thisBit) & other._binRepr[i]) |
                (BigUnsignedInt::_carryFlag & ((1 ^ thisBit) ^ other._binRepr[i]));
    }
    return *this;
}

//TODO: test it and check that _carryBit is set right in cases of overflow
template<std::size_t N>
BigUnsignedInt<N>& BigUnsignedInt<N>::operator*=(const BigUnsignedInt& other)
{
    BigUnsignedInt thisCopy(*this);
    *this = BigUnsignedInt();
    std::size_t prev = N - 1; // position of the end of number after last shift

    for (std::size_t i = N - 1; i != std::numeric_limits<BigUnsignedInt::size_type>::max(); --i)
    {
        if (other._binRepr[i] == 1)
        {
            // Shift the thisCopy so the last digit of original *this stays in i-th place
            thisCopy <<= prev - i;
            //if(BigUnsignedInt::_carryFlag == 1) // check that no overflow happened
            //return *this;

            *this += thisCopy;
            //if(BigUnsignedInt::_carryFlag == 1)
            //return *this;

            prev = i;
        }
    }

    return *this;
}

template<std::size_t N>
BigUnsignedInt<N>::BigUnsignedInt(const std::string& stringRepr) : BigUnsignedInt()
{
    createFromString(stringRepr, *this);
}

template<std::size_t N>
void BigUnsignedInt<N>::createFromString(const std::string& input, BigUnsignedInt& bigUnsignedInt)
{
    using namespace std;
    regex unsignedNumber("[[:digit:]]+");
    if (!regex_match(input, unsignedNumber))
        throw invalid_argument("provided input isn't unsigned integer");

    // Fill queue with digits of provided number
    queue<typename BigUnsignedInt<N>::Unsigned> digits;
    for (const auto& letter:input)
        digits.push(static_cast<typename BigUnsignedInt<N>::Unsigned>(letter - '0'));

    deque<typename BigUnsignedInt<N>::Unsigned> quotient;
    typename BigUnsignedInt<N>::size_type counter = N - 1;

    // Convert provided number to binary
    do
    {
        typename BigUnsignedInt<N>::Unsigned value = 0;
        while (!digits.empty())
        {
            value = value * 10 + digits.front();
            digits.pop();
            quotient.push_back(value / 2);
            value %= 2;

            if (counter == numeric_limits<decltype(counter)>::max())
                throw invalid_argument("provided number is too large");
        }
        bigUnsignedInt._binRepr[counter] = value;
        --counter;

        while (!quotient.empty() && quotient.front() == 0)
            quotient.pop_front();
        while (!quotient.empty())
        {
            digits.push(quotient.front());
            quotient.pop_front();
        }
    } while (!digits.empty());
}


#endif //BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_HPP
