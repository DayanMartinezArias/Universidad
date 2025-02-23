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
  bool operator==(const BigInteger& obj) const;

  BigInteger<Base> operator+(const BigInteger<Base>& obj) const;
  BigInteger<Base> operator-(const BigInteger<Base>& obj) const;
  BigInteger<Base> operator/(const BigInteger<Base>& obj) const;
  BigInteger<Base> operator%(const BigInteger<Base>& obj) const;

  BigInteger<Base>& operator++(); // Pre-incremento
  BigInteger<Base> operator++(int a); // Post-incremento
  BigInteger<Base>& operator--(); // Pre-decremento
  BigInteger<Base> operator--(int b); // Post-decremento

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
    return !sign_; 
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

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator+(const BigInteger<Base>& obj) const {
  if (sign_ == obj.sign_) {
    BigInteger<Base> res(value_ + obj.value_);
    res.sign_ = sign_;
    return res;
  } 

  if (value_ < obj.value_) {
    BigInteger res(obj.value_ - value_);
    res.sign_ = obj.sign_;
    return res;
  } else if (value_ >= obj.value_) {
    BigInteger res(value_ - obj.value_);
    res.sign_ = sign_;
    if (value_ == obj.value_) {
      res.sign_ = true;
    }
    return res;
  }
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator/(const BigInteger<Base>& obj) const {
  BigInteger res(value_ / obj.value_);
  if (sign_ == obj.sign_) {
    res.sign_ = true;
  } else {
    res.sign_ = false;
  }
  return res;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator-(const BigInteger<Base>& obj) const {
  BigInteger<Base> copy(obj.value_);
  copy.sign_ = !obj.sign_;
  return *this + copy;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator%(const BigInteger<Base>& obj) const {
  BigUnsigned<Base> result = value_ % obj.value_;  
  BigInteger<Base> modResult(result);
  modResult.sign_ = sign_; 
  return modResult; 
}

template <unsigned char Base>
bool BigInteger<Base>::operator==(const BigInteger& obj) const {
  return sign_ == obj.sign_ && value_ == obj.value_;
}

template <unsigned char Base>
BigInteger<Base>& BigInteger<Base>::operator++() {
  if (sign_) { 
    value_ = value_ + 1;
  } else {  
    value_ = value_ - 1;
    if (value_ == BigUnsigned<Base>()) {
      sign_ = true; 
    }
  }
  return *this;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator++(int) {
  BigInteger<Base> temp = *this;
  ++(*this); 
  return temp;
}

template <unsigned char Base>
BigInteger<Base>& BigInteger<Base>::operator--() {
  if (sign_) {  
    if (value_ == BigUnsigned<Base>()) {
      sign_ = false;  
      value_ = 1;
    } else {
      value_ = value_ - 1;
    }
  } else { 
    value_ = value_ + 1;
  }
  return *this;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator--(int) {
  BigInteger<Base> temp = *this;
  --(*this); 
  return temp;
}

template<>
class BigInteger<2> {
 public:
  void show() {std::cout << "A" << std::endl;};
};


#endif