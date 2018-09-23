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
BigUnsignedInt<N> operator+(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b);

template<std::size_t N>
BigUnsignedInt<N> operator-(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b);

template<std::size_t N = BIG_UNSIGNED_INT_SIZE>
class BigUnsignedInt {
public:

    using Unsigned = unsigned int;
    using UnsignedVector = std::vector<Unsigned>;
    using size_type = UnsignedVector::size_type;

    friend std::istream& operator>><N>(std::istream& is, BigUnsignedInt<N>& bigUnsignedInt);

    friend std::ostream& operator<<<N>(std::ostream& os, const BigUnsignedInt<N>& bigUnsignedInt);

    friend std::ostream& operator<<<N>(std::ostream& os, BigUnsignedInt<N>&& bigUnsignedInt);

    friend BigUnsignedInt<N> operator+<N>(const BigUnsignedInt& a, const BigUnsignedInt& b);

    friend BigUnsignedInt<N> operator-<N>(const BigUnsignedInt& a, const BigUnsignedInt& b);

public:

    BigUnsignedInt() : _binRepr(N) {}

    static bool overflowed() { return _carryFlag > 0; }

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
std::ostream& operator<<(std::ostream& os, BigUnsignedInt<N>&& bigUnsignedInt) { os << bigUnsignedInt; }

template<std::size_t N>
BigUnsignedInt<N> operator+(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b)
{
    BigUnsignedInt<N>::_carryFlag = 0;
    BigUnsignedInt<N> result;
    for (typename BigUnsignedInt<N>::size_type i = N - 1;
         i != std::numeric_limits<typename BigUnsignedInt<N>::size_type>::max(); --i)
    {
        result._binRepr[i] = a._binRepr[i] ^ b._binRepr[i] ^ BigUnsignedInt<N>::_carryFlag;
        BigUnsignedInt<N>::_carryFlag =
                (a._binRepr[i] & b._binRepr[i]) | (BigUnsignedInt<N>::_carryFlag & (a._binRepr[i] ^ b._binRepr[i]));
    }
    return result;
}

template<std::size_t N>
BigUnsignedInt<N> operator-(const BigUnsignedInt<N>& a, const BigUnsignedInt<N>& b)
{
    BigUnsignedInt<N>::_carryFlag = 0;
    BigUnsignedInt<N> result;
    for (typename BigUnsignedInt<N>::size_type i = N - 1;
         i != std::numeric_limits<typename BigUnsignedInt<N>::size_type>::max(); --i)
    {
        result._binRepr[i] = a._binRepr[i] ^ b._binRepr[i] ^ BigUnsignedInt<N>::_carryFlag;
        BigUnsignedInt<N>::_carryFlag =
                ((1 ^ a._binRepr[i]) & b._binRepr[i]) |
                (BigUnsignedInt<N>::_carryFlag & ((1 ^ a._binRepr[i]) ^ b._binRepr[i]));
    }
    return result;
}

#endif //BIGUNSIGNEDINTEGER_BIGUNSIGNEDINT_HPP
