#include "BigUnsigned.h"
#include "BigInt.h"

BigInteger gcd(BigInteger a, BigInteger b) {
  a = a.abs(); 
  b = b.abs();

  while (!(b == BigInteger(0))) {
    BigInteger temp = b;
    b = a % b;
    a = temp;
  }
  return a;
}



int main (int argc, unsigned char* argv[]) {
  BigInteger a;
  BigInteger b;
  std::cin >> a;
  std::cin >> b;
  BigInteger c = gcd(a, b);
  std::cout << "Greatest common divisor is: " << c << std::endl;
}