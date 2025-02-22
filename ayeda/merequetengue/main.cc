#include <iostream>
#include "big_unsigned.h"
#include "big_integer.h"

int main () {
  BigInteger<16> z;
  std::cin >> z;
  std::cout << z << std::endl;

}