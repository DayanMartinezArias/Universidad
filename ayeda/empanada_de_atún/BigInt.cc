#include "BigInt.h"
#include <cmath>
#include <sstream>

BigInteger::BigInteger(const BigUnsigned& value) : value_(value), sign_(true) {}

BigInteger::BigInteger(int n) : value_(std::fabs(n)) {
  if (n < 0) this->sign_ = false;
  else this->sign_ = true;
} 

BigInteger& BigInteger::operator-() {
  this->sign_ = !this->sign_;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& obj) {
  char sign{'\0'};
  if(!obj.sign_)  {
    sign = '-';
  }
  os << sign << obj.value_;
  return os;
}

std::istream& operator>>(std::istream& is, BigInteger& obj) {
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
  if (obj.value_ == BigUnsigned() && obj.sign_ == false) {
    obj.sign_ = true;
  }
  return is;
}

bool BigInteger::operator==(const BigInteger& obj) const {
  return sign_ == obj.sign_ && value_ == obj.value_;
}

bool operator<(const BigInteger& left, const BigInteger& right) {
  if (left.sign_ != right.sign_) {
    return !left.sign_; 
  } 
 
  if (left.sign_) {
    return left.value_ < right.value_; 
  } else {
    return !(left.value_ < right.value_); 
  }
}

// To add two BigInts
BigInteger operator+(const BigInteger& left, const BigInteger& right) {
  if (left.sign_ == right.sign_) {
    std::cout << left.value_ << " " << right.value_ << std::endl;
    BigInteger res(left.value_ + right.value_);
    res.sign_ = left.sign_;
    return res;
  } 

  if (left.value_ < right.value_) {
    BigInteger res(right.value_ - left.value_);
    res.sign_ = right.sign_;
    return res;
  } else if (left.value_ >= right.value_) {
    BigInteger res(left.value_ - right.value_);
    res.sign_ = left.sign_;
    if (left.value_ == right.value_) {
      res.sign_ = true;
    }
    return res;
  }
}

// To substract two BigInts
BigInteger BigInteger::operator-(const BigInteger& obj) const {
  BigInteger copy(obj.value_);
  copy.sign_ = !obj.sign_;
  return *this + copy;
}

// To multiply two BigInts
BigInteger BigInteger::operator*(const BigInteger& obj) const {
  BigInteger res(value_ * obj.value_);
  if (sign_ == obj.sign_) {
    res.sign_ = true;
  } else {
    res.sign_ = false;
  }

  return res;
}

// To divide two BigInts
BigInteger operator/(const BigInteger& left, const BigInteger& right) {
  BigInteger res(left.value_ / right.value_);
  if (left.sign_ == right.sign_) {
    res.sign_ = true;
  } else {
    res.sign_ = false;
  }

  return res;
}

// Returns the mod
BigInteger BigInteger::operator%(const BigInteger& obj) const {
  BigUnsigned result = value_ % obj.value_;  
  BigInteger modResult(result);
  modResult.sign_ = sign_; 
  return modResult;
}

// Pre-incremento (++a)
BigInteger& BigInteger::operator++() {
  if (sign_) { 
    value_ = value_ + 1;
  } else {  
    value_ = value_ - 1;
    if (value_ == BigUnsigned()) {
      sign_ = true; 
    }
  }
  return *this;
}

// Post-incremento (a++)
BigInteger BigInteger::operator++(int) {
  BigInteger temp = *this;
  ++(*this); 
  return temp;
}

// Pre-decremento (--a)
BigInteger& BigInteger::operator--() {
  if (sign_) {  
    if (value_ == BigUnsigned()) {
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

// Post-decremento (a--)
BigInteger BigInteger::operator--(int) {
  BigInteger temp = *this;
  --(*this); 
  return temp;
}

// returns the absolute value of the number
BigInteger BigInteger::abs() const {
  BigInteger result = *this;
  result.sign_ = true; 
  return result;
}

