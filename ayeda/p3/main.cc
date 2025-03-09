#include <iostream>

#include "big_number.h"
#include "big_rational.h"


int main () {
  const char* ab = "60/6r";
  auto bn1 = BigNumber<10>::create(ab);
  bn1->write(std::cout);
  BigInteger<10> a("8i");
  a.write(std::cout);
  -a;
  a.add(*bn1)->write(std::cout);
  a.write(std::cout);
}