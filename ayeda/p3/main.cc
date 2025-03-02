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
  BigNumber<10>* bn1 = BigNumber<10>::create("7382i");
  BigUnsigned<10> a = GetType(bn1);
  a.write(std::cout);
  a.GetVectorOfDigits();
}