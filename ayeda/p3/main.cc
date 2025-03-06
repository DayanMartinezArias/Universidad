#include <iostream>

#include "big_number.h"
#include "big_rational.h"

template <unsigned char Base>
BigUnsigned<Base> GetType (BigNumber<Base>* obj) {
  if (auto new_obj = dynamic_cast<BigUnsigned<Base>*>(obj)) {
    BigUnsigned<Base> res = (BigUnsigned<Base>) *new_obj;
    return res;
  }
  if (auto new_obj = dynamic_cast<BigInteger<Base>*>(obj)) {
    BigUnsigned<Base> res = (BigUnsigned<Base>) *new_obj;
    return res;
  }
}

int main () {
  auto bn1 = BigNumber<10>::create("60u");
  BigNumber<10>* bn2 = BigNumber<10>::create("0008i");
  (GetType(bn1) % GetType(bn2)).write(std::cout);
  BigInteger<10> a("7i");
  a.write(std::cout);
  -a;
  a.add(*bn1)->write(std::cout);
  a.write(std::cout);
}