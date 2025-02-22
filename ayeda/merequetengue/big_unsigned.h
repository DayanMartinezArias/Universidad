#ifndef BIG_UNSIGNED_H
#define BIG_UNSIGNED_H

#include <vector>
#include <iostream>
#include <algorithm>

template <unsigned char Base = 10>
class BigUnsigned {
 public:
  
  // Constructores
  BigUnsigned(unsigned number = 0);
  BigUnsigned(const char* number);
  BigUnsigned(const BigUnsigned<Base>& number); 

  // Operaciones artméticas
  BigUnsigned<Base> operator+(const BigUnsigned<Base> obj) const;
  BigUnsigned<Base> operator-(const BigUnsigned<Base>& obj) const;
  BigUnsigned<Base> operator*(const BigUnsigned<Base>& obj) const;
  BigUnsigned<Base> operator/(const BigUnsigned<Base>& obj) const;
  BigUnsigned<Base> operator%(const BigUnsigned<Base>& obj) const;
  BigUnsigned<Base>& operator=(const BigUnsigned<Base>& obj);

  // Operaciones lógicas
  bool operator<(const BigUnsigned<Base>& obj) const;
  bool operator>=(const BigUnsigned<Base>& obj) const;
  bool operator==(const BigUnsigned<Base>& obj) const;

  // Operaciones unarias
  BigUnsigned<Base>& operator++();
  BigUnsigned<Base> operator++(int);
  BigUnsigned<Base>& operator--();
  BigUnsigned<Base> operator--(int);

  // Operador de extracción
  friend std::ostream& operator<<(std::ostream& os, const BigUnsigned<Base>& number) {
    for (const char digit : number.digits_) {
      os << digit;
    }
    return os;
  }
  
  // Operador de inserción
  friend std::istream& operator>>(std::istream& is, BigUnsigned<Base>& number) {
    std::string line;
    is >> line;


    line.erase(0, line.find_first_not_of('0'));

    if (line.empty()) {
      number = BigUnsigned<Base>();
      return is;
    }
    
    number.digits_.clear();
    
    for (const char& digit : line) {
      if(!number.ValidateChar(digit)) throw std::invalid_argument("Invalid character for base");
      else number.digits_.emplace_back(digit);
    }   

    return is;
  }

 private:
  bool ValidateChar(const char digit) const;
  std::vector<char> digits_;
};

template <unsigned char Base>
BigUnsigned<Base>::BigUnsigned(unsigned number) {
  std::string str = std::to_string(number);
  for (const char& digit: str) {
    if(!ValidateChar(digit)) throw std::invalid_argument("Invalid character for base");
    else digits_.emplace_back(digit);
  } 
}

template <unsigned char Base>
bool BigUnsigned<Base>::ValidateChar(const char c) const {
  if constexpr (Base == 2) {
    return (c == '0' || c == '1');
  } else if constexpr (Base == 8) {
    return (c >= '0' && c <= '7');
  } else if constexpr (Base == 10) {
    return (c >= '0' && c <= '9');
  } else if constexpr (Base == 16) {
    return ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F'));
  }
  return false;
}

template <unsigned char Base>
BigUnsigned<Base>::BigUnsigned(const BigUnsigned<Base>& number) : digits_(number.digits_) {}

template <unsigned char Base>
BigUnsigned<Base>::BigUnsigned(const char* number) {
  static_assert(Base == 2 || Base == 8 || Base == 10 || Base == 16, "Base must be 2, 8, 10, or 16");
  while (*number != '\0') {
    if (!ValidateChar(*number)) throw std::invalid_argument("Invalid character for base");
    else digits_.emplace_back(*number);
    ++number;
  }
}

unsigned charToValue(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return 10 + (c - 'A');
    if (c >= 'a' && c <= 'f') return 10 + (c - 'a'); 
    throw std::invalid_argument("Invalid character");
}

char valueToChar(unsigned val) {
    if (val < 10) return '0' + val;
    if (val < 16) return 'A' + (val - 10);
    throw std::invalid_argument("Out of range value");
}

template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator+(const BigUnsigned<Base> obj) const {
  std::vector<char> vec;
  unsigned sum;
  unsigned carry = 0;

  int left_index = digits_.size() - 1;
  int right_index = obj.digits_.size() - 1;

  while (left_index >= 0 || right_index >= 0 || carry > 0) {
    unsigned left_digit = (left_index >= 0) ? charToValue(digits_[left_index]) : 0;
    unsigned right_digit = (right_index >= 0) ? charToValue(obj.digits_[right_index]) : 0;

    sum = left_digit + right_digit + carry;
    carry = sum / Base;
    sum = sum % Base;

    vec.emplace_back(valueToChar(sum));

    --left_index;
    --right_index;
  }

  std::reverse(vec.begin(), vec.end());

  BigUnsigned<Base> result;
  result.digits_.assign(vec.begin(), vec.end());
  return result;
}

template <unsigned char Base>
BigUnsigned<Base>& BigUnsigned<Base>::operator=(const BigUnsigned<Base>& obj) {
  if (this != &obj) {
    digits_ = obj.digits_;
  }
  return *this;
}

template <unsigned char Base>
bool BigUnsigned<Base>::operator==(const BigUnsigned<Base>& obj) const {
  if (digits_.size() != obj.digits_.size()) {
    return false;
  } else {
    for (size_t i{0}; i < digits_.size(); ++i) {
      if (digits_[i] != obj.digits_[i]) {
        return false;
      }
    }
  }
  return true;
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
    unsigned left_digit = charToValue(digits_[left_index]);
    unsigned right_digit = (right_index >= 0) ? charToValue(obj.digits_[right_index]): 0;

    if (left_digit < right_digit + carry) {
      left_digit += Base;
      sub = left_digit - right_digit - carry;
      carry = 1;
    } else {
      sub = left_digit - right_digit - carry;
      carry = 0;
    }
    vec.emplace_back(valueToChar(sub));

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

// Multiplying two BigUnsigned
template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator*(const BigUnsigned<Base>& obj) const {
  BigUnsigned<Base> acc;
  BigUnsigned<Base> aux;

  std::vector<unsigned> aux_vec;
  unsigned carry = 0;

  // Go through each element of obj in reverse
  // works fine
  for (int i{obj.digits_.size() -1}; i >= 0; --i) {
    aux_vec.clear();
    unsigned bottom = charToValue(obj.digits_[i]);

    //Adds zeros at the end of a vector
    // works fine
    for (int k{0}; k < obj.digits_.size() -1 -i; ++k ) {
      aux_vec.push_back(0);
    }
    // Go through each element of this in reverse
    // wroks fine
    for (int j{digits_.size() - 1}; j >= 0; --j) {
      unsigned top = digits_[j] - '0';
      unsigned sum = (top * bottom) + carry;
      carry = sum / 10;
      sum = sum % 10;
      aux_vec.push_back(sum);
    }
    if (carry > 0) {
      aux_vec.push_back(carry);
    }

    carry = 0;
    
    aux.digits_.clear();
    for (const unsigned digit : aux_vec) {
      aux.digits_.push_back(digit + '0');
    }
    std::reverse(aux.digits_.begin(), aux.digits_.end());
    
    acc = acc + aux;
  }
  return acc;
}

template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator%(const BigUnsigned<Base>& obj) const {
  if (obj == BigUnsigned<Base>()) {
    throw std::invalid_argument("Cannot divide by zero");
  } else if (*this < obj) {
    return *this; 
  }

  BigUnsigned<Base> remainder;

  for (size_t i = 0; i < digits_.size(); ++i) {
    remainder.digits_.push_back(digits_[i]);

    while (remainder.digits_.size() > 1 && remainder.digits_[0] == '0') {
      remainder.digits_.erase(remainder.digits_.begin());
    }

    while (remainder >= obj) {
      remainder = remainder - obj;
    }
  }

  while (remainder.digits_.size() > 1 && remainder.digits_[0] == '0') {
    remainder.digits_.erase(remainder.digits_.begin());
  }
  return remainder;
}

template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator/(const BigUnsigned<Base>& obj) const {
  if (obj == BigUnsigned<Base>()) {
    throw std::invalid_argument("cannot divide by zero");
  } else if (*this < obj) {
    return BigUnsigned<Base>();
  } else if (*this == obj) {
    return BigUnsigned<Base>("1");
  }

  BigUnsigned<Base> quotient;
  BigUnsigned<Base> remainder;

  for (size_t i = 0; i < digits_.size(); ++i) {
    remainder.digits_.push_back(digits_[i]);

    while (remainder.digits_.size() > 1 && remainder.digits_[0] == '0') {
      remainder.digits_.erase(remainder.digits_.begin());
    }

    unsigned count = 0;
    while ((remainder >= obj)) {
      remainder = remainder - obj;
      ++count;
    }

    quotient.digits_.push_back(valueToChar(count));
  }

  while (quotient.digits_.size() > 1 && quotient.digits_[0] == '0') {
    quotient.digits_.erase(quotient.digits_.begin());
  }

  return quotient;
}

template <unsigned char Base>
bool BigUnsigned<Base>::operator<(const BigUnsigned<Base>& obj) const {
  if ((*this >= obj)) return false;
  else return true;
}

template <unsigned char Base>
bool BigUnsigned<Base>::operator>=(const BigUnsigned<Base>& obj) const {
  if (digits_.size() > obj.digits_.size()) {
    return true;
  } else if (digits_.size() < obj.digits_.size()) {
    return false;
  }
  
  for (size_t i = 0; i < digits_.size(); ++i) {
    unsigned left = charToValue(digits_[i]);
    unsigned right = charToValue(obj.digits_[i]);
    if (left > right) {
      return true;
    } else if (left < right) {
      return false;
    }
  }
  return true;
}


// Pre-incremento (++a)
template <unsigned char Base>
BigUnsigned<Base>& BigUnsigned<Base>::operator++() {
  *this = *this + 1;
  return *this;
}

// Post-incremento (a++)
template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator++(int) {
  BigUnsigned temp = *this;
  ++(*this);  // Reutiliza el operador de pre-incremento
  return temp;
}

// Pre-decremento (--a)
template <unsigned char Base>
BigUnsigned<Base>& BigUnsigned<Base>::operator--() {
  if (*this == BigUnsigned()) {
    throw std::underflow_error("BigUnsigned cannot be decremented below zero");
  }
  *this = *this - 1;  
  return *this;
}

// Post-decremento (a--)
template <unsigned char Base>
BigUnsigned<Base> BigUnsigned<Base>::operator--(int) {
  BigUnsigned temp = *this;
  --(*this); 
  return temp;
}


#endif
