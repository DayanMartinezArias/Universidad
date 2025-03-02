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

  BigUnsigned<Base> abs() const {return abs_;}
  bool GetSign() const {return sign_;}

  BigNumber<Base>& divide(const BigNumber<Base>& passed_obj) const override {}

  /*virtual BigNumber<Base>& add(const BigNumber<Base>& obj) const {};
  virtual BigNumber<Base>& subtract(const BigNumber<Base>&) const {};
  virtual BigNumber<Base>& multiply(const BigNumber<Base>&) const {};
  virtual BigNumber<Base>& divide(const BigNumber<Base>&) const {};*/

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

#endif