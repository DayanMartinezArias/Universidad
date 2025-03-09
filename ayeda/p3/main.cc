#include <iostream>

#include "big_number.h"
#include "big_rational.h"


int main () {
  const char* ab = "-60/20r";
  auto bn1 = BigNumber<10>::create(ab);
  auto bn2 = BigNumber<10>::create("9u");
  auto bn3 = BigNumber<10>::create("-9i");
  bn1->write(std::cout);
  bn3->write(std::cout);
  BigInteger<10> a("8i");
  a.write(std::cout);
  -a;
  a.add(*bn1)->write(std::cout);
  a.write(std::cout);
  bn2->add(*bn3)->write(std::cout);
}