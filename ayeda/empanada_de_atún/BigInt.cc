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
  line.erase(0, line.find_first_not_of('0'));
  std::istringstream input(line);
  input >> obj.value_;
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

BigInteger BigInteger::operator-(const BigInteger& obj) const {
  BigInteger copy(obj.value_);
  copy.sign_ = !obj.sign_;
  return *this + copy;
}

BigInteger BigInteger::operator*(const BigInteger& obj) const {
  BigInteger res(value_ * obj.value_);
  if (sign_ == obj.sign_) {
    res.sign_ = true;
  } else {
    res.sign_ = false;
  }

  return res;
}

BigInteger operator/(const BigInteger& left, const BigInteger& right) {
  BigInteger res(left.value_ / right.value_);
  if (left.sign_ == right.sign_) {
    res.sign_ = true;
  } else {
    res.sign_ = false;
  }

  return res;
}

BigInteger BigInteger::operator%(const BigInteger& obj) const {
  BigUnsigned result = value_ % obj.value_;  
  BigInteger modResult(result);
  modResult.sign_ = sign_; 
  return modResult;
}

// Pre-incremento (++a)
BigInteger& BigInteger::operator++() {
  if (sign_) {  // Si es positivo
    value_ = value_ + 1;
  } else {  // Si es negativo
    value_ = value_ - 1;
    if (value_ == BigUnsigned()) {
      sign_ = true;  // -1 + 1 = 0, cambia el signo a positivo
    }
  }
  return *this;
}

// Post-incremento (a++)
BigInteger BigInteger::operator++(int) {
  BigInteger temp = *this;
  ++(*this);  // Reutiliza el pre-incremento
  return temp;
}

// Pre-decremento (--a)
BigInteger& BigInteger::operator--() {
  if (sign_) {  // Si es positivo
    if (value_ == BigUnsigned()) {
      sign_ = false;  // 0 - 1 = -1
      value_ = 1;
    } else {
      value_ = value_ - 1;
    }
  } else {  // Si es negativo
    value_ = value_ + 1;
  }
  return *this;
}

// Post-decremento (a--)
BigInteger BigInteger::operator--(int) {
  BigInteger temp = *this;
  --(*this);  // Reutiliza el pre-decremento
  return temp;
}

BigInteger BigInteger::abs() const {
  BigInteger result = *this;
  result.sign_ = true;  // Convertimos a positivo
  return result;
}

