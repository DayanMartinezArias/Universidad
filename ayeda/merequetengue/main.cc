#include <iostream>
#include "big_unsigned.h"

int main () {;
  BigUnsigned<16> a;
  BigUnsigned<16> c;

  std::cin >> a;
  std::cin >> c;

  std::cout << a / c << std::endl;
  std::cout << a % c << std::endl;
}