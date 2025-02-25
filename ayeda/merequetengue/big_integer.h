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
  BigInteger<Base> operator*(const BigInteger<Base>& obj) const;
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
  if (Base != 10) throw std::invalid_argument("Constructor only valid for base 10 numbers");
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
BigInteger<Base> BigInteger<Base>::operator*(const BigInteger<Base>& obj) const {
  BigInteger<Base> res(value_ * obj.value_);
  if (sign_ == obj.sign_) {
    res.sign_ = true;
  } else {
    res.sign_ = false;
  }
  return res; 
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
    value_ = value_ + BigUnsigned(1);
  } else {  
    value_ = value_ - BigUnsigned(1);
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
      value_ = BigUnsigned(1);
    } else {
      value_ = value_ - BigUnsigned(1);
    }
  } else { 
    value_ = value_ + BigUnsigned(1);
  }
  return *this;
}

template <unsigned char Base>
BigInteger<Base> BigInteger<Base>::operator--(int) {
  BigInteger<Base> temp = *this;
  --(*this); 
  return temp;
}

// Especialización para los números binarios
template<>
class BigInteger<2> {
 public:
  BigInteger(const BigUnsigned<2>& value);
  BigInteger(int n = 0) : binary_digits(n) {};
  
  BigInteger<2> twos_complement() const;
  bool operator==(const BigInteger<2>& obj) const;
  BigInteger<2> operator-(const BigInteger<2>& obj) const;
  BigInteger<2> operator+(const BigInteger<2>& obj) const;
  BigInteger<2> operator*(const BigInteger<2>& obj) const;
  
  friend std::istream& operator>>(std::istream& is, BigInteger<2>& obj) {
    std::string line;
    is >> line;

    for (const char& digit : line) {
      if (digit != '1' && digit != '0') throw std::invalid_argument("Not valid character for base");
      obj.binary_digits.emplace_back(digit == '1');
    }
    return is;
  }

  friend std::ostream& operator<<(std::ostream& os, const BigInteger<2>& obj) {
    for (const bool& digits : obj.binary_digits) {
      os << (digits == true ? '1' : '0');
    }
    return os;
  }

 private:
  std::vector<bool> binary_digits;
};

BigInteger<2>::BigInteger(const BigUnsigned<2>& value) {
  std::ostringstream oss;
  oss << value;
  std::string value_str = oss.str();
  for (const char& digit : value_str) {
    binary_digits.emplace_back(digit == '1');
  }
}

bool BigInteger<2>::operator==(const BigInteger<2>& obj) const {
  return binary_digits == obj.binary_digits;
}

BigInteger<2> BigInteger<2>::twos_complement() const {
  BigInteger<2> result = *this;

  // 1. Invertir todos los bits
  for (size_t i = 0; i < result.binary_digits.size(); ++i) {
    result.binary_digits[i] = !result.binary_digits[i];
  }

  // 2. Sumar 1 al resultado
  BigInteger<2> one;
  one.binary_digits.push_back(1); // Representa el número "1" en binario
  return result + one; // Usa la suma ya implementada
}


BigInteger<2> BigInteger<2>::operator+(const BigInteger<2>& obj) const {
  BigInteger<2> result;
  
  int size_lhs = binary_digits.size() - 1;
  int size_rhs = obj.binary_digits.size() - 1;
  
  bool carry = false;
  bool can_overflow = false; 
  if (binary_digits[size_lhs] == binary_digits[size_rhs]) can_overflow = true;

  while (size_lhs >= 0 || size_rhs >= 0) {
    bool left = size_lhs >= 0 ? binary_digits[size_lhs] : false;
    bool right = size_rhs >= 0 ? obj.binary_digits[size_rhs] : false;

    bool bit_sum = left ^ right;
    bool bit_n_carry = bit_sum ^ carry;
    
    carry = (left && right) || (carry && (left || right));
    
    result.binary_digits.emplace_back(bit_n_carry);
    
    size_lhs--;
    size_rhs--;
  }
  if (result.binary_digits[result.binary_digits.size() - 1] != binary_digits[0]) {
    result.binary_digits.emplace_back(binary_digits[0]);
  }
  std::reverse(result.binary_digits.begin(), result.binary_digits.end());
  
  return result;
}

BigInteger<2> BigInteger<2>::operator-(const BigInteger<2>& obj) const {
    return *this + obj.twos_complement();
}

BigInteger<2> BigInteger<2>::operator*(const BigInteger<2>& obj) const {
    BigInteger<2> result; 
    BigInteger<2> multiplicand = *this; 
    BigInteger<2> multiplier = obj;

    result.binary_digits.resize(multiplicand.binary_digits.size() + multiplier.binary_digits.size(), false);

    for (int i = 0; i < multiplier.binary_digits.size(); ++i) {
      if (multiplier.binary_digits[i]) { 
      for (int j = 0; j < multiplicand.binary_digits.size(); ++j) {
        if (multiplicand.binary_digits[j]) {
          result.binary_digits[i + j] = result.binary_digits[i + j] ^ true;
        }
      }
    }
  }
  if (std::all_of(result.binary_digits.begin(), result.binary_digits.end(), [](bool bit) { return !bit; })) {
    result.binary_digits.push_back(false);
  }
  return result;
}


#endif