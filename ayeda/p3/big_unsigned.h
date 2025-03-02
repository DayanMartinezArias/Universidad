#ifndef BIG_UNSIGNED_H
#define BIG_UNSIGNED_H

#include <iostream>
#include <vector>
#include "big_number.h"
#include "big_integer.h"

template <unsigned char Base>
class BigNumber;

template <unsigned char Base>
class BigInteger;

template <unsigned char Base>
class BigUnsigned : public BigNumber<Base> {
 public:
  ~BigUnsigned() override {};
  BigUnsigned(const char* number);
  std::vector<char> GetVectorOfDigits() const;

  /*virtual BigNumber<Base>& add(const BigNumber<Base>& obj) const;
  virtual BigNumber<Base>& subtract(const BigNumber<Base>&) const {};
  virtual BigNumber<Base>& multiply(const BigNumber<Base>&) const {};
  virtual BigNumber<Base>& divide(const BigNumber<Base>&) const {};*/

  operator BigUnsigned<Base>() const override {return *this;};
  operator BigInteger<Base>() const {return BigInteger<Base>(*this);};
  // operator Bigrational<Base>() const;
  

  std::ostream& write(std::ostream& os) const override;
  std::istream& read(std::istream&) override;

  static unsigned CharToVal(const char& digit);
  static char ValToChar(const unsigned& value);

 private:
  std::vector<char> digits_;
};

template <unsigned char Base>
std::vector<char> BigUnsigned<Base>::GetVectorOfDigits() const {
  return digits_;
}

template <unsigned char Base>
unsigned BigUnsigned<Base>::CharToVal(const char& digit) { 
  if (digit >= '0' && digit <= '9') {
    return digit - '0';
  }
  if (Base > 10 && digit >= 'A' && digit <= 'Z') {
    return digit - 'A' + 10;
  }
}

template <unsigned char Base>
char BigUnsigned<Base>::ValToChar(const unsigned& value) {
  if (value < 10) {
    return '0' + value;
  }
  if (Base > 10 && value < Base) {
    return 'A' + (value - 10);
  }
}

template <unsigned char Base>
BigUnsigned<Base>::BigUnsigned(const char* number) {
  // Comppribación de caracteres válidos
  while (*number != 'u') {
    digits_.emplace_back(*number);
    number++;
  }
}

template <unsigned char Base>
std::ostream& BigUnsigned<Base>::write(std::ostream& os) const {
  for (const char& digit : digits_) {
    os << digit;
  }
  os << "\n";
  return os;
}

template <unsigned char Base>
std::istream& BigUnsigned<Base>::read(std::istream& is) {
  std::string line;
  is >> line;
  
  // Compribación de valor de base y caracteres válidos
  int inx{0};
  while (inx < line.length() - 2) {
    digits_[inx] = line[inx];
    ++inx;
  }
  return is;
}

#endif