#include "BigUnsignedInt.hpp"

using namespace std;

int main()
{
    constexpr size_t N = 8 * sizeof(unsigned long long);
    string number1, number2;
    cin >> number1 >> number2;

    BigUnsignedInt<N> bi1(number1), bi2(number2);
    BigUnsignedInt<N> bi3 = bi1 * bi2;
    cout << bi1 << "\n"
         << bi2 << "\n"
         << bi3 << "\n"
         << "Overflowed: " << (BigUnsignedInt<N>::overflowed() ? "yes" : "no") << endl;

    unsigned long long u1, u2;
    u1 = stoull(number1);
    u2 = stoull(number2);
    cout << BigUnsignedInt<N>(to_string(u1 * u2)) << endl;
    return 0;
}