#ifndef BIG_RATIONAL_H
#define BIG_RATIONAL_H

#include "big_number.h"
#include "big_integer.h"
#include "big_unsigned.h"

template <unsigned char Base>
class BigNumber;

template <unsigned char Base>
class BigInteger;

template <unsigned char Base>
class BigRational : public BigNumber<Base> {
 public: 
  BigRational(const BigInteger<Base> numerator, const BigInteger<Base> denominator);

  BigInteger<Base> GetNumerator() const {return numerator_;}
  BigInteger<Base> GetDenominator() const {return denominator_;}

  BigInteger<Base> mcd(BigInteger<Base> numerator, BigInteger<Base> denominator) const;
  BigInteger<Base> mcd() const;
  void Minimize();

  bool operator==(const BigRational<Base>& obj) const;
  bool operator<(const BigRational<Base>& obj) const;
  BigRational<Base>& operator=(const BigRational<Base>& other);

  BigRational<Base> operator+(const BigRational<Base>& obj) const;

  operator BigUnsigned<Base>() const {}
  operator BigInteger<Base>() const {}

  virtual std::ostream& write(std::ostream& os) const override;
  virtual std::istream& read(std::istream& is) {};

 private:
  BigInteger<Base> numerator_;
  BigInteger<Base> denominator_;
};

template <unsigned char Base>
BigRational<Base>::BigRational(const BigInteger<Base> numerator, const BigInteger<Base> denominator) : numerator_(numerator), denominator_(denominator) {
  if (!(numerator.GetSign() || denominator.GetSign())) {
    -numerator_;
    -denominator_;
  } else if (denominator.GetSign() == false) {
    -numerator_;
    -denominator_;   
  }
}

template <unsigned char Base>
std::ostream& BigRational<Base>::write(std::ostream& os) const {
  if (!numerator_.GetSign()) os << '-';
  for (const char& digit : numerator_.abs().GetVectorOfDigits()) {
    os << digit;
  }

  os << " / ";

  if (!denominator_.GetSign()) os << '-';
  for (const char& digit : denominator_.abs().GetVectorOfDigits()) {
    os << digit;
  }
  os << "\n";
  return os;
}

template <unsigned char Base>
BigInteger<Base> BigRational<Base>::mcd() const {
  return mcd(GetNumerator(), GetDenominator());
}

template <unsigned char Base>
BigInteger<Base> BigRational<Base>::mcd(BigInteger<Base> numerator, BigInteger<Base> denominator) const {
  while (!(denominator.abs() == BigUnsigned<Base>("0u"))) { 
    BigInteger<Base> temp = denominator;
    denominator = numerator % denominator;
    numerator = temp;
  }
  return BigInteger<Base>(numerator.abs());
}

template <unsigned char Base>
void BigRational<Base>::Minimize() {
  BigInteger<Base> gcd = mcd();
  numerator_ = numerator_ / gcd;  
  denominator_ = denominator_ / gcd;
}

template <unsigned char Base>
BigRational<Base> BigRational<Base>::operator+(const BigRational<Base>& obj) const {
  BigInteger<Base> new_denominator = denominator_ * obj.denominator_;
  BigInteger<Base> new_numerator = numerator_ * obj.denominator_ + obj.numerator_ * denominator_;

  BigRational<Base> result(new_numerator, new_denominator);
  result.Minimize();
  return result;
}

template <unsigned char Base>
bool BigRational<Base>::operator==(const BigRational<Base>& obj) const {
  return numerator_ * obj.denominator_ == obj.numerator_ * denominator_;
}


template <unsigned char Base>
bool BigRational<Base>::operator<(const BigRational<Base>& obj) const {
  return numerator_ * obj.denominator_ < obj.numerator_ * denominator_;
}

template <unsigned char Base>
BigRational<Base>& BigRational<Base>::operator=(const BigRational<Base>& other) {
  if (this != &other) {  
    this->numerator_ = other.numerator_;
    this->denominator_ = other.denominator_;
  }
  return *this;
}


#endif