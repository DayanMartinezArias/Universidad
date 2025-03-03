#ifndef BIG_NUMBER_H
#define BIG_NUMBER_H

#include <cstring>
#include "big_unsigned.h"
#include "big_rational.h"

template <unsigned char Base>
class BigInteger;

template <unsigned char Base>
class BigRational;

template <unsigned char Base>
class BigUnsigned; // Forward declaration

template <unsigned char Base>
class BigNumber {
 public:
  virtual ~BigNumber() {};
  
  virtual operator BigUnsigned<Base>() const = 0;
  virtual operator BigInteger<Base>() const = 0;
  //virtual operator Bigrational<Base>() const = 0;
  

  virtual std::ostream& write(std::ostream& os) const = 0;
  virtual std::istream& read(std::istream& is) = 0;

  static BigNumber<Base>* create(const char* string_of_digits) {
    if (!string_of_digits || *string_of_digits == '\0') return nullptr;

    size_t len = std::strlen(string_of_digits);
    char type = string_of_digits[len - 1]; 
    // que pasa si no especifíco base
    switch (type) {
      case 'u' :
        return new BigUnsigned<Base>(string_of_digits);
      case 'i' :
         return new BigInteger<Base>(BigUnsigned<Base>(string_of_digits));
      case 'r' :
         return new BigRational<Base>(BigInteger<Base>(string_of_digits), BigInteger<Base>(string_of_digits));
      default:
        return nullptr;
    }
  }
};

#endif