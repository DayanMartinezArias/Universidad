#ifndef BIG_UNSIGNED_H
#define BIG_UNSIGNED_H

#include <iostream>
#include <vector>
#include <algorithm>

#include "big_number.h"
#include "big_integer.h"

template <unsigned char Base>
class BigNumber;

template <unsigned char Base>
class BigInteger;

template <unsigned char Base>
class BigUnsigned : public BigNumber<Base> {
 public:
  ~BigUnsigned() override {};
  BigUnsigned(const char* number);
  BigUnsigned() : digits_({'0'}) {}

  std::vector<char> GetVectorOfDigits() const;

  bool operator==(const BigUnsigned<Base>& obj) const;
  bool operator<(const BigUnsigned<Base>& obj) const;

  BigUnsigned<Base>& operator=(const BigUnsigned<Base>& other);

  operator BigUnsigned<Base>() const override {return *this;};
  operator BigInteger<Base>() const override {return BigInteger<Base>(*this);};
  // operator Bigrational<Base>() const;

  BigUnsigned<Base> operator+(const BigUnsigned<Base>& obj) const;
  BigUnsigned<Base> operator-(const BigUnsigned<Base>& obj) const;
  BigUnsigned<Base> operator*(const BigUnsigned<Base>& obj) const;
  BigUnsigned<Base> operator/(const BigUnsigned<Base>& obj) const;
  
  std::ostream& write(std::ostream& os) const override;
  std::istream& read(std::istream&) override;

  static unsigned CharToVal(const char& digit);
  static char ValToChar(const unsigned& value);

 private:
  std::vector<char> digits_;
};

template <unsigned char Base>
std::vector<char> BigUnsigned<Base>::GetVectorOfDigits() const {
  return digits_;
}

template <unsigned char Base>
unsigned BigUnsigned<Base>::CharToVal(const char& digit) { 
  if (digit >= '0' && digit <= '9') {
    return digit - '0';
  }
  if (Base > 10 && digit >= 'A' && digit <= 'Z') {
    return digit - 'A' + 10;
  }
}

template <unsigned char Base>
char BigUnsigned<Base>::ValToChar(const unsigned& value) {
  if (value < 10) {
    return '0' + value;
  }
  if (Base > 10 && value < Base) {
    return 'A' + (value - 10);
  }
}

template <unsigned char Base>
BigUnsigned<Base>::BigUnsigned(const char* number) {
  // Comppribación de caracteres válidos
  while (*number == '0') {
    number++;
  }
  while (*number != 'u' && *number != 'i' && *number != '\n') {
    digits_.emplace_back(*number);
    number++;
  }
  if (digits_.empty()) digits_ = {'0'};
}

template <unsigned char Base>
std::ostream& BigUnsigned<Base>::write(std::ostream& os) const {
  for (const char& digit : digits_) {
    os << digit;
  }
  os << "\n";
  return os;
}

template <unsigned char Base>
std::istream& BigUnsigned<Base>::read(std::istream& is) {
  std::string line;
  is >> line;
  
  // Compribación de valor de base y caracteres válidos
  int inx{0};
  while (inx < line.length() - 2) {
    digits_[inx] = line[inx];
    ++inx;
  }
  return is;
}

template <unsigned char Base>
bool BigUnsigned<Base>::operator==(const BigUnsigned<Base>& obj) const {
  if (digits_.size() != obj.digits_.size()) return false;
  for (size_t i{0}; i < digits_.size(); ++i) {
    if (digits_[i] != obj.digits_[i]) return false;
  }
  return true;
}

template <unsigned char Base>
bool BigUnsigned<Base>::operator<(const BigUnsigned<Base>& obj) const {
  if (digits_.size() != obj.digits_.size()) {
    return digits_.size() < obj.digits_.size();
  }

  for (size_t i = 0; i < digits_.size(); ++i) {
    if (CharToVal(digits_[i]) != CharToVal(obj.digits_[i])) {
      return CharToVal(digits_[i]) < CharToVal(obj.digits_[i]);
    }
  }
  
  return false;
}

template <unsigned char Base>
BigUnsigned<Base>& BigUnsigned<Base>::operator=(const BigUnsigned<Base>& other) {
  if (this != &other) {  
    this->digits_ = other.digits_;
  }
  return *this;
}

template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator+(const BigUnsigned<Base>& obj) const {
  std::vector<char> vec;
  unsigned sum;
  unsigned carry = 0;

  int left_index = digits_.size() - 1;
  int right_index = obj.digits_.size() - 1;

  while (left_index >= 0 || right_index >= 0 || carry > 0) {
    unsigned left_digit = (left_index >= 0) ? CharToVal(digits_[left_index]) : 0;
    unsigned right_digit = (right_index >= 0) ? CharToVal(obj.digits_[right_index]) : 0;
    sum = left_digit + right_digit + carry;
    carry = sum / Base;
    sum = sum % Base;

    vec.emplace_back(ValToChar(sum));

    --left_index;
    --right_index;
  }

  std::reverse(vec.begin(), vec.end());

  BigUnsigned<Base> result;
  result.digits_.assign(vec.begin(), vec.end());
  return result;
}

template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator-(const BigUnsigned<Base>& obj) const {
 if (digits_.size() < obj.digits_.size()) throw std::invalid_argument("Left side of the operation cannot be greater than right side");

 std::vector<char> vec;
  unsigned sub;
  unsigned carry = 0;

  int left_index = digits_.size() - 1;
  int right_index = obj.digits_.size() - 1;

  while (left_index >= 0 || right_index >= 0) {
    unsigned left_digit = CharToVal(digits_[left_index]);
    unsigned right_digit = (right_index >= 0) ? CharToVal(obj.digits_[right_index]): 0;

    if (left_digit < right_digit + carry) {
      left_digit += Base;
      sub = left_digit - right_digit - carry;
      carry = 1;
    } else {
      sub = left_digit - right_digit - carry;
      carry = 0;
    }
    vec.emplace_back(ValToChar(sub));

    --left_index;
    --right_index;
  } 
  if (carry == 1) throw std::invalid_argument("Left side of the operation cannot be greater than right side");
  
  for (size_t i{vec.size() -1}; i >= 1; --i) {
    if (vec[i] == '0') vec.pop_back();
    else break;
  }

  std::reverse(vec.begin(), vec.end()); 
  BigUnsigned<Base> result;
  result.digits_.assign(vec.begin(), vec.end());
  return result;
}

template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator*(const BigUnsigned<Base>& obj) const {
  BigUnsigned<Base> acc("0u"); 
  BigUnsigned<Base> aux("0u");
  std::vector<unsigned> aux_vec;
  unsigned carry = 0;

  for (int i{obj.digits_.size() - 1}; i >= 0; --i) {
    aux_vec.clear();
    unsigned bottom = CharToVal(obj.digits_[i]);

    // Add trailing zeros (shifting effect)
    aux_vec.insert(aux_vec.end(), obj.digits_.size() - 1 - i, 0);

    // Multiply each digit of `this` by `bottom`
    for (int j{digits_.size() - 1}; j >= 0; --j) {
      unsigned top = CharToVal(digits_[j]);
      unsigned sum = (top * bottom) + carry;
      carry = sum / Base;
      sum = sum % Base;
      aux_vec.push_back(sum);
    }

    if (carry > 0) {
      aux_vec.push_back(carry);
      carry = 0;
    }

    // Store aux_vec into aux
    aux.digits_.clear();
    for (const unsigned digit : aux_vec) {
      aux.digits_.push_back(ValToChar(digit));
    }
    
    // Reverse only if necessary
    std::reverse(aux.digits_.begin(), aux.digits_.end());

    auto it = std::find_if(aux.digits_.begin(), aux.digits_.end(), [](char x) { return x != '0'; });
    aux.digits_.erase(aux.digits_.begin(), it);
    acc = acc + aux;  // Accumulate result
  }
  return acc;
}

template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator/(const BigUnsigned<Base>& obj) const {
  if (obj == BigUnsigned<Base>("0u")) {
    throw std::invalid_argument("cannot divide by zero");
  } else if (*this < obj) {
    return BigUnsigned<Base>();
  } else if (*this == obj) {
    return BigUnsigned<Base>("1u");
  }

  BigUnsigned<Base> quotient;
  BigUnsigned<Base> remainder;

  for (size_t i = 0; i < digits_.size(); ++i) {
    remainder.digits_.push_back(digits_[i]);

    while (remainder.digits_.size() > 1 && remainder.digits_[0] == '0') {
      remainder.digits_.erase(remainder.digits_.begin());
    }

    unsigned count = 0;
    while (!(remainder < obj)) {
      remainder = remainder - obj;
      ++count;
    }

    quotient.digits_.push_back(ValToChar(count));
  }

  while (quotient.digits_.size() > 1 && quotient.digits_[0] == '0') {
    quotient.digits_.erase(quotient.digits_.begin());
  }

  return quotient;
}

#endif