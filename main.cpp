#include "BigUnsignedInt.hpp"

using namespace std;


int main()
{
    BigUnsignedInt<64> bi1, bi2;
    cin >> bi1;
    cin >> bi2;
    BigUnsignedInt<64> bi3 = bi1 + bi2;
    cout << bi1 << "\n";
    cout << bi2 << "\n";
    cout << bi3 << endl;
    return 0;
}