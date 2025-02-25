#include <iostream>
#include "big_unsigned.h"
#include "big_integer.h"
#include "big_rational.h"

int main () {
  BigRational<10> a;
  BigRational<10> b;
  std::cin >> a;
  std::cin >> b;
  std::cout << a + b << std::endl;
}