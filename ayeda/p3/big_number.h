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
  virtual operator BigRational<Base>() const = 0;
  
  virtual BigNumber<Base>* add(const BigNumber<Base>& obj) const = 0;

  virtual std::ostream& write(std::ostream& os) const = 0;
  virtual std::istream& read(std::istream& is) = 0;

  static BigNumber<Base>* create(const char* string_of_digits) {
    if (!string_of_digits || *string_of_digits == '\0') return nullptr;

    size_t len = std::strlen(string_of_digits);
    char type = string_of_digits[len - 1]; 
    std::string a(string_of_digits);
    // que pasa si no especifíco base
    if (type == 'u') return new BigUnsigned<Base>(string_of_digits);
    else if (type == 'i') return new BigInteger<Base>(BigUnsigned<Base>(string_of_digits));
    else if (type == 'r') {
      const char *delimiter = strchr(string_of_digits, '/');
      if (delimiter) {
        const char *first = string_of_digits;  // First part
        const char *second = delimiter + 1;  // Second part
        BigInteger<Base> n(first);
        BigInteger<Base> d(second);
        return new BigRational<Base>(n, d);
      }
    }
    else return nullptr;
  }
};

#endif