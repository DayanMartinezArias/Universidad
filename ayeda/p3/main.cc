#include <iostream>

#include "big_number.h"
#include "big_rational.h"

int main () {
  BigUnsigned<10> c("12u");
  c.write(std::cout);
  BigInteger<10> d = (BigInteger<10>)c;
  -d;
  d.write(std::cout);
  BigRational<10> m(d, d);
  m.GetDenominator().write(std::cout);
  m.GetNumerator().write(std::cout);
  m.write(std::cout);
}