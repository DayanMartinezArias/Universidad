#include <iostream>

#include "big_number.h"
#include "big_rational.h"


int main () {
  const char* ab = "-60/15r";
  auto bn1 = BigNumber<10>::create(ab);
  auto bn2 = BigNumber<10>::create("9u");
  auto bn3 = BigNumber<10>::create("-9i");
  bn1->write(std::cout);
  bn2->write(std::cout);
  bn3->write(std::cout);
  
  bn2->sub(*bn3)->write(std::cout);
}