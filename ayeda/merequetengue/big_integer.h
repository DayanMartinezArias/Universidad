#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <cmath>
#include <sstream>
#include "big_unsigned.h"

template <unsigned char Base = 10>
class BigInteger {
 public:
  BigInteger(const BigUnsigned<Base>& value);
  BigInteger(int n = 0); 

  BigInteger<Base>& operator-();
  bool operator<(const BigInteger<Base>& obj) const;

  friend std::ostream& operator<<(std::ostream& os, const BigInteger<Base>& obj) {
    char sign{'\0'};
    if(!obj.sign_)  {
      sign = '-';
    }
    os << sign << obj.value_;
    return os;
  }

  friend std::istream& operator>>(std::istream& is, BigInteger<Base>& obj) {
    std::string line;
    is >> line;
    if (line[0] == '-') {
      obj.sign_ = false;
      line = line.substr(1, line.length() - 1);
    } else {
      obj.sign_ = true;
    }
   // line.erase(0, line.find_first_not_of('0'));
    std::istringstream input(line);
    input >> obj.value_;
    if (obj.value_ == BigUnsigned<Base>() && obj.sign_ == false) {
      obj.sign_ = true;
    }
    return is;
  }

 private:
  BigUnsigned<Base> value_;
  bool sign_;
};

template <unsigned char Base>
bool BigInteger<Base>::operator<(const BigInteger<Base>& obj) const {
  if (sign_ != obj.sign_) {
    return !left.sign_; 
  } 

  if (sign_) {
    return value_ < obj.value_; 
  } else {
    return !(value_ < obj.value_); 
  }
}

template <unsigned char Base>
BigInteger<Base>::BigInteger(const BigUnsigned<Base>& value) : value_(value), sign_(true) {}

template <unsigned char Base>
BigInteger<Base>::BigInteger(int n) : value_(std::fabs(n)) {
  if (n < 0) this->sign_ = false;
  else this->sign_ = true;
} 

template <unsigned char Base>
BigInteger<Base>& BigInteger<Base>::operator-() {
  if (value_ == BigUnsigned<Base>()) throw std::invalid_argument("Cannot negate zero");
  this->sign_ = !this->sign_;
  return *this;
}


#endif