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

  BigInteger<Base> mcd(const BigInteger<Base>& numerator, const BigInteger<Base>& denominator) const;
  BigInteger<Base> mcd() const;

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
BigInteger<Base> BigRational<Base>::mcd(const BigInteger<Base>& numerator, const BigInteger<Base>& denominator) const {
if (GetDenominator() == BigUnsigned<Base>("0u")) return numerator_;
else return mcd(GetDenominator(), GetNumerator() % GetDenominator());
}

#endif