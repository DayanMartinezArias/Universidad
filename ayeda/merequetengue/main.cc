#include <iostream>
#include "big_unsigned.h"
#include "big_integer.h"

int main () {
  BigInteger<16> z;
  BigInteger<16> w;
  std::cin >> z;
  std::cin >> w;
  std::cout << z / w << std::endl;
  std::cout << z % w << std::endl;
}