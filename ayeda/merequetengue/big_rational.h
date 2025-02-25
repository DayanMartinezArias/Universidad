#ifndef BIG_RATIONAL_H
#define BIG_RATIONAL_H

#include "big_integer.h"

template <unsigned char Base>
class BigRational {
 public:
  BigRational(const BigInteger<Base>& numerador = 0, const BigUnsigned<Base>& denominador=1);

  BigRational<Base> operator*(const BigRational<Base>& obj) const;
  BigRational<Base> operator+(const BigRational<Base>& obj) const;

  friend std::ostream& operator<<(std::ostream& os, const BigRational<Base>& obj) {
   os << obj.numerador_ << "/" << obj.denominador_;
   return os;
  }

  friend std::istream& operator>>(std::istream& is, BigRational<Base>& obj) {
    is >> obj.numerador_;
    is >> obj.denominador_;
    if (obj.denominador_ == BigUnsigned("0")) throw std::invalid_argument("Cannot initialize object with denominator being zero");
    obj.MinimumFraction();
    return is;
  }

 private:
  BigUnsigned<Base> MCD(BigUnsigned<Base> num, BigUnsigned<Base> den) const;
  void MinimumFraction();
  BigInteger<Base> numerador_;
  BigUnsigned<Base> denominador_;
};


template <unsigned char Base>
BigRational<Base>::BigRational(const BigInteger<Base>& numerador, const BigUnsigned<Base>& denominador) : numerador_(numerador), denominador_(denominador) {
  if (denominador_ == BigUnsigned("0")) throw std::invalid_argument("Cannot initialize object with denominator being zero");
  MinimumFraction();
}

// MCD usando el algoritmo de Euclides
template <unsigned char Base>
BigUnsigned<Base> BigRational<Base>::MCD(BigUnsigned<Base> num, BigUnsigned<Base> den) const {
  while (!(den == BigUnsigned<Base>("0"))) { 
    BigUnsigned<Base> temp = den;
    den = num % den;
    num = temp;
  }
  return num;
}

// Reducir la fracción
template <unsigned char Base>
void BigRational<Base>::MinimumFraction() {
  BigUnsigned<Base> mcd = MCD(numerador_.GetUnsigned(), denominador_);
  numerador_ = numerador_ / BigInteger<Base>(mcd);  
  denominador_ = denominador_ / mcd;
}

template <unsigned char Base>
BigRational<Base> BigRational<Base>::operator*(const BigRational<Base>& obj) const {
  return BigRational<Base>(denominador_ * obj.denominador_, numerador_ * obj.numerador_);
}

template <unsigned char Base>
BigRational<Base> BigRational<Base>::operator+(const BigRational<Base>& obj) const {
  BigUnsigned<Base> new_denominador = denominador_ * obj.denominador_;
  BigInteger<Base> new_numerador = numerador_ * obj.denominador_ + obj.numerador_ * denominador_;

  if (new_numerador < 0) {
    new_numerador = -new_numerador;
  }

  BigRational<Base> result(new_numerador, new_denominador);
  result.MinimumFraction();
  return result;
}

#endif