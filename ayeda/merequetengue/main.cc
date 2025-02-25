#include <iostream>
#include "big_unsigned.h"
#include "big_integer.h"

int main () {
  BigInteger<2> z;
  BigInteger<2> w;
  std::cin >> z;
  std::cin >> w;
  std::cout << z << std::endl;
  std::cout << w << std::endl;
  std::cout << z + w << std::endl;
}