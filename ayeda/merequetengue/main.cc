#include <iostream>
#include "big_unsigned.h"
#include "big_integer.h"

int main () {
  BigInteger<2> z;
  BigInteger<2> w;
  std::cout << z << std::endl;
  std::cin >> z;
  std::cin >> w;
}