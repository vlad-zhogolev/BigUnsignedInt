//
// Created by ZV on 22.09.2018.
//

#include "BigUnsignedInt.h"

template<std::size_t N>
typename BigUnsignedInt<N>::Unsigned BigUnsignedInt<N>::_carryFlag = 0;

template<std::size_t N>
std::istream& operator>>(std::istream& is, BigUnsignedInt<N>& bigUnsignedInt)
{
    // Check that provided input is unsigned number
    std::string input;
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

template<std::size_t N>
BigUnsignedInt<N>& BigUnsignedInt<N>::operator<<=(const std::size_t shift)
{
    _carryFlag = 0;

    // Shift completely erases number
    if (shift >= N)
    {
        for (Unsigned& bit:_binRepr)
            bit = 0;
        _carryFlag = 1;
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
        _carryFlag = 1;

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
    _carryFlag = 0;
    for (size_type i = N - 1; i != std::numeric_limits<size_type>::max(); --i)
    {
        Unsigned thisBit = this->_binRepr[i];
        _binRepr[i] = thisBit ^ other._binRepr[i] ^ _carryFlag;
        _carryFlag = (thisBit & other._binRepr[i]) | (_carryFlag & (thisBit ^ other._binRepr[i]));
    }
    return *this;
}

template<std::size_t N>
BigUnsignedInt<N>& BigUnsignedInt<N>::operator-=(const BigUnsignedInt& other)
{
    _carryFlag = 0;
    for (size_type i = N - 1; i != std::numeric_limits<size_type>::max(); --i)
    {
        Unsigned thisBit = _binRepr[i];
        _binRepr[i] = thisBit ^ other._binRepr[i] ^ _carryFlag;
        thisBit ^= 1;
        _carryFlag = (thisBit & other._binRepr[i]) | (_carryFlag & (thisBit ^ other._binRepr[i]));
    }
    return *this;
}

template<std::size_t N>
BigUnsignedInt<N>& BigUnsignedInt<N>::operator*=(const BigUnsignedInt& other)
{
    Unsigned multOverflow = 0;
    BigUnsignedInt thisCopy(*this);
    this->resetToZero();
    std::size_t prev = N - 1; // position of the end of number represented by *this after last shift

    for (std::size_t i = N - 1; i != std::numeric_limits<size_type>::max(); --i)
    {
        if (other._binRepr[i] == 1)
        {
            // Shift the thisCopy so the last digit of original *this stays in i-th place
            thisCopy <<= prev - i;
            multOverflow |= _carryFlag;
            *this += thisCopy;
            multOverflow |= _carryFlag;
            prev = i;
        }
    }
    _carryFlag = multOverflow;

    return *this;
}

template<std::size_t N>
BigUnsignedInt<N>::BigUnsignedInt(const std::string& stringRepr) : BigUnsignedInt()
{
    createFromString(stringRepr, *this);
}

template<std::size_t N>
void BigUnsignedInt<N>::resetToZero() {
    for(Unsigned& bit:_binRepr)
        bit = 0;
}

template<std::size_t N>
void BigUnsignedInt<N>::createFromString(const std::string& input, BigUnsignedInt& bigUnsignedInt)
{
    using namespace std;

    // Check input
    regex unsignedNumber("[[:digit:]]+");
    if (!regex_match(input, unsignedNumber))
        throw invalid_argument("provided input isn't unsigned integer");

    // Fill dividend with digits of provided number
    queue<Unsigned> dividend;
    for (const auto& letter:input)
        dividend.push(static_cast<Unsigned>(letter - '0'));

    deque<Unsigned> quotient;
    size_type counter = N - 1;

    // Convert provided number to binary
    do
    {
        Unsigned value = 0;
        while (!dividend.empty())
        {
            value = value * 10 + dividend.front();
            dividend.pop();
            quotient.push_back(value / 2);
            value %= 2;

            if (counter == numeric_limits<size_type>::max())
                throw invalid_argument("provided number is too large");
        }
        bigUnsignedInt._binRepr[counter] = value;
        --counter;

        while (!quotient.empty() && quotient.front() == 0)
            quotient.pop_front();
        while (!quotient.empty())
        {
            dividend.push(quotient.front());
            quotient.pop_front();
        }
    } while (!dividend.empty());
}