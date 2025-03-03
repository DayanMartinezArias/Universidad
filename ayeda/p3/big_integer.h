#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <iostream>
#include "big_number.h"
#include "big_unsigned.h"

template <unsigned char Base>
class BigNumber;

template <unsigned char Base>
class BigUnsigned;


template <unsigned char Base>
class BigInteger : public BigNumber<Base> {
 public:
  ~BigInteger() {};
  BigInteger(const BigUnsigned<Base> abs) : abs_(abs), sign_(true) {};

  BigInteger<Base>& operator-();
  BigInteger<Base> operator-() const;

  bool operator==(const BigInteger<Base>& obj) const;
  bool operator<(const BigInteger<Base>& obj) const;
  BigInteger<Base>& operator=(const BigInteger<Base>& other);

  BigInteger<Base> operator+(const BigInteger<Base>& obj) const;
  BigInteger<Base> operator-(const BigInteger<Base>& obj) const;
  BigInteger<Base> operator*(const BigInteger<Base>& obj) const;
  BigInteger<Base> operator/(const BigInteger<Base>& obj) const;
  BigInteger<Base> operator%(const BigInteger<Base>& obj) const;

  BigUnsigned<Base> abs() const {return abs_;}
  bool GetSign() const {return sign_;}

  operator BigUnsigned<Base>() const override {return abs_;};
  operator BigInteger<Base>() const {return *this;};
  
  virtual std::ostream& write(std::ostream& os) const override;
  virtual std::istream& read(std::istream& is) {};
 private:
  BigUnsigned<Base> abs_;
  bool sign_;
};

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator-() const {
  BigInteger ret(abs_);
  ret.sign_ = sign_;
  return ret;
}

template <unsigned char Base>
BigInteger<Base>& BigInteger<Base>::operator-() {
  sign_ = !sign_;
  return *this;
}

template <unsigned char Base>
std::ostream& BigInteger<Base>::write(std::ostream& os) const {
  if (!sign_) os << '-';
  for (const char& digit : abs_.GetVectorOfDigits()) {
    os << digit;
  }
  os << "\n";
  return os;
}

template <unsigned char Base>
bool BigInteger<Base>::operator==(const BigInteger<Base>& obj) const {
  return (sign_ == obj.sign_) && (abs() == obj.abs());
}

template <unsigned char Base>
bool BigInteger<Base>::operator<(const BigInteger<Base>& obj) const {
  if (sign_ != obj.sign_) return !sign_;
  if (sign_) return abs() < obj.abs();
  else return (!(abs() < obj.abs()));
}

template <unsigned char Base>
BigInteger<Base>& BigInteger<Base>::operator=(const BigInteger<Base>& other) {
  if (this != &other) {  
    this->digits_ = other.digits_;
    this->sign_ = other.sign_;
  }
  return *this;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator-(const BigInteger<Base>& obj) const {
  BigInteger<Base> result("0i");

  if (sign_ == obj.sign_) {
    if (!(abs() < obj.abs())) {
      result.abs_ = abs() - obj.abs();
      result.sign_ = sign_;  
    } else {
      result.abs_ = obj.abs() - abs();
      result.sign_ = !sign_; 
    }
  } else {
    result.abs_ = abs() + obj.abs();
    result.sign_ = sign_;
  }
  if (result.abs_ == BigUnsigned<Base>("0u")) {
    result.sign_ = true;
  }

  return result;
}


template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator+(const BigInteger<Base>& obj) const {
  BigInteger<Base> result("0i");
  if (sign_ == obj.sign_) {
    result.sign_ = sign_;
    result.abs_ = abs() + obj.abs();
  }
  else {
    if (abs() < obj.abs()) {
      result.sign_ = obj.sign_;
      result.abs_ =  obj.abs() - abs();
    }
    else {
      result.sign_ = sign_;
      result.abs_ =  abs() - obj.abs();
    }
  }
  if (abs() == obj.abs()) {
    result.sign_ = true;
  }
  return result;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator*(const BigInteger<Base>& obj) const {
  BigInteger<Base> res("0i");
  res.abs_ = abs() * obj.abs();
  res.sign_ = !(sign_ ^ obj.sign_);
  if (res.abs() == BigUnsigned<Base>("0u")) {
    res.sign_ = true;
  }
  return res;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator/(const BigInteger<Base>& obj) const {
  BigInteger<Base> res("0i");
  res.abs_ = abs() / obj.abs();
  res.sign_ = !(sign_ ^ obj.sign_);
  if (res.abs() == BigUnsigned<Base>("0u")) {
    res.sign_ = true;
  }
  return res;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator%(const BigInteger<Base>& obj) const {
  if (obj.abs() == BigUnsigned<Base>("0u")) {
    throw std::runtime_error("Modulo by zero is undefined.");
  }

  BigUnsigned<Base> remainder = abs() % obj.abs();

  BigInteger<Base> rem(remainder);
  rem.sign_ = sign_;
  return rem;
}


#endif