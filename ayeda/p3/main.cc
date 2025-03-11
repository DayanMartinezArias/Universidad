#include <iostream>

#include "big_number.h"
#include "big_rational.h"


int main () {
  const char* ab = "1/1r";
  auto bn1 = BigNumber<10>::create(ab);
  auto bn2 = BigNumber<10>::create("9u");
  auto bn3 = BigNumber<10>::create("-3i");
   
  std::cout << *bn3 << std::endl;
  std::cin >> *bn3;
  std::cout << *bn3 << std::endl;
  
  auto op1 = bn3->divide(*bn1);
  
  
  delete op1;
  delete bn1;
  delete bn2;
  delete bn3;

  std::cout << "Nuevas cosas" << std::endl;

  BigRational<10> a(BigInteger<10>("0i"), BigInteger<10>("1i"));
  a.read(std::cin);
  a.write(std::cout);
  

  return 0;
}