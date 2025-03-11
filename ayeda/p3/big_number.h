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
  virtual BigNumber<Base>* sub(const BigNumber<Base>& obj) const = 0;
  virtual BigNumber<Base>* multiplicate(const BigNumber<Base>& obj) const = 0;
  virtual BigNumber<Base>* divide(const BigNumber<Base>& obj) const = 0;

  static BigNumber<Base>* create(const char* string_of_digits) {
    if (!string_of_digits || *string_of_digits == '\0') return nullptr;

    size_t len = std::strlen(string_of_digits);
    char type = string_of_digits[len - 1]; 
    // que pasa si no especifíco base
    if (type == 'u') return new BigUnsigned<Base>(string_of_digits);
    else if (type == 'i') {
      bool ne = true;
      if (string_of_digits[0] == '-') {
        ne = false;
        string_of_digits++;
        return new BigInteger<Base>(BigUnsigned<Base>(string_of_digits), ne);
      }
      return new BigInteger<Base>(BigUnsigned<Base>(string_of_digits));
    }
    else if (type == 'r') { 
      bool ne = true;
      if (string_of_digits[0] == '-') {
        ne = false;
        string_of_digits++;
      }
      const char *delimiter = strchr(string_of_digits, '/');
      if (delimiter) {
        const char *first = string_of_digits; 
        const char *second = delimiter + 1;
        BigInteger<Base> n(first, ne);
        BigInteger<Base> d(second);
        return new BigRational<Base>(n, d);
      }
    }
    else return nullptr;
  }

  friend std::ostream& operator<<(std::ostream& os, BigNumber<Base>& obj) {
    obj.write(os);
    return os;
  }
  friend std::istream& operator>>(std::istream& is, BigNumber<Base>& obj) {
    obj.read(is);
    return is;
  }

 protected:
  virtual std::ostream& write(std::ostream& os) const = 0;
  virtual std::istream& read(std::istream& is) = 0;
};

#endif