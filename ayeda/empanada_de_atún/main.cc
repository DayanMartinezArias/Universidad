#include "BigUnsigned.h"
#include "BigInt.h"

int main (int argc, char* argv[]) {

/*BigInteger a;
BigInteger b;
std::cin >> a;
std::cin >> b;
std::cout << a << std::endl;
std::cout << a * b << std::endl;
*/
unsigned char* ab = new unsigned char[9];
ab[0] = '0';
ab[1] = '0';
ab[2] = '9';
ab[3] = '9';

BigUnsigned a(ab);
std::cout << a << std::endl;
}