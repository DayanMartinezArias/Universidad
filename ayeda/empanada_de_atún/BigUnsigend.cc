#include <algorithm>
#include <cstring>
#include <string>
#include <algorithm>

#include "BigUnsigned.h"

//isidid
void IsDigit(unsigned char digit) {
  if (!std::isdigit(digit)) {
    std::cout << "Invalid character: '" << digit << "' (ASCII: " << static_cast<int>(digit) << ")" << std::endl;
    throw std::invalid_argument("Argument is not a digit");
  }
}


std::vector<unsigned char> ConvertIntToChar(unsigned number) {
  std::string str = std::to_string(number);
  std::vector<unsigned char> result;
  for (const char& c : str) {
    result.push_back(static_cast<unsigned char>(c));
  }
  return result;
}

// Constructor from an unsigned
// Defaukt value is zero
BigUnsigned::BigUnsigned(unsigned number) {
  std::vector<unsigned char> digits = ConvertIntToChar(number);
  for (const unsigned char& digit: digits) {
    IsDigit(digit);
    digits_.emplace_back(digit);
  }
}

// Constructor that accepts an array of chars
BigUnsigned::BigUnsigned(const unsigned char* digits) {
  while (*digits == '0') {
    digits++;
  }

  while (*digits != '\0') {
    IsDigit(*digits);
    digits_.emplace_back(*digits);
    digits++;
  }
}

// Copy constructor
BigUnsigned::BigUnsigned(const BigUnsigned& obj) {
  digits_ = obj.digits_;
}

// Overload of assigment operator
BigUnsigned& BigUnsigned::operator=(const BigUnsigned& obj) {
  if (this != &obj) {
    digits_ = obj.digits_;
  }
  return *this;
}

// For printing to the terminal
std::ostream& operator<<(std::ostream& os, const BigUnsigned& obj) {
  for (const unsigned char& digit : obj.digits_) {
    os << digit;
  }
  return os;
}

// For reading an object of the class
std::istream& operator>>(std::istream& is, BigUnsigned& obj) {
  obj.digits_.clear();
  std::string number_line;
  is >> number_line;

  number_line.erase(0, number_line.find_first_not_of('0'));

  if (number_line.empty()) {
    number_line = "0";
  }
  
  for (const char& digit : number_line) {
    IsDigit(static_cast<unsigned char>(digit));
    obj.digits_.emplace_back(static_cast<unsigned char>(digit));
  }
  return is;
}

// Comparision between two BigUnsigned
bool BigUnsigned::operator==(const BigUnsigned& obj) const {
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

// Less than comparation
bool operator<(const BigUnsigned& left, const BigUnsigned& right) {
  if ((left >= right)) return false;
  else return true;
}

// Grater equal comparation
bool BigUnsigned::operator>=(const BigUnsigned& other) const {
  if (digits_.size() > other.digits_.size()) {
    return true;
  } else if (digits_.size() < other.digits_.size()) {
    return false;
  }

  for (size_t i = 0; i < digits_.size(); ++i) {
    if (digits_[i] > other.digits_[i]) {
      return true;
    } else if (digits_[i] < other.digits_[i]) {
      return false;
    }
  }

  return true;
}

// Adding two BigUnsigned
BigUnsigned operator+(const BigUnsigned& left, const BigUnsigned& right) {
  std::vector<unsigned> vec;
  unsigned sum;
  unsigned carry = 0;

  int left_index = left.digits_.size() - 1;
  int right_index = right.digits_.size() - 1;

  while (left_index >= 0 || right_index >= 0) {
    unsigned left_digit = (left_index >= 0) ? left.digits_[left_index] - '0': 0;
    unsigned right_digit = (right_index >= 0) ? right.digits_[right_index] -'0': 0;

    sum = left_digit + right_digit + carry;
    carry = sum / 10;
    sum = sum % 10;
    
    vec.emplace_back(sum);

    --left_index;
    --right_index;
  }
  if (carry > 0) {
    vec.emplace_back(carry);
  }

  std::reverse(vec.begin(), vec.end()); 
  BigUnsigned result;
  result.digits_.clear();
  for (const unsigned& digit : vec) {
    result.digits_.emplace_back(digit + '0');
  }
  return result;
}

// Adding two BigUnsigned
BigUnsigned BigUnsigned::operator-(const BigUnsigned& obj) const {
 if (digits_.size() < obj.digits_.size()) throw std::invalid_argument("Left side of the operation cannot be greater than right side");

 std::vector<unsigned> vec;
  unsigned sub;
  unsigned carry = 0;

  int left_index = digits_.size() - 1;
  int right_index = obj.digits_.size() - 1;

  while (left_index >= 0 || right_index >= 0) {
    unsigned left_digit = digits_[left_index] - '0';
    unsigned right_digit = (right_index >= 0) ? obj.digits_[right_index] - '0': 0;

    if (left_digit < right_digit + carry) {
      left_digit += 10;
      sub = left_digit - right_digit - carry;
      carry = 1;
    } else {
      sub = left_digit - right_digit - carry;
      carry = 0;
    }
    vec.emplace_back(sub);

    --left_index;
    --right_index;
  } 
  if (carry == 1) throw std::invalid_argument("Left side of the operation cannot be greater than right side");

  for (size_t i{vec.size() -1}; i >= 1; --i) {
    if (vec[i] == 0) vec.pop_back();
    else break;
  }

  std::reverse(vec.begin(), vec.end()); 
  BigUnsigned result;
  result.digits_.clear();
  for (const unsigned& digit : vec) {
    result.digits_.emplace_back(digit + '0');
  }
  return result;
}

// Multiplying two BigUnsigned
BigUnsigned BigUnsigned::operator*(const BigUnsigned& obj) const {
  BigUnsigned acc;
  BigUnsigned aux;

  std::vector<unsigned> aux_vec;
  unsigned carry = 0;

  // Go through each element of obj in reverse
  // works fine
  for (int i{obj.digits_.size() -1}; i >= 0; --i) {
    aux_vec.clear();
    unsigned bottom = obj.digits_[i] - '0';

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

// Dividing two BigUnsigned
BigUnsigned operator/(const BigUnsigned& left, const BigUnsigned& right) {
  if (right == BigUnsigned()) {
    throw std::invalid_argument("cannot divide by zero");
  } else if (left < right) {
    return BigUnsigned();
  } else if (left == right) {
    return BigUnsigned(1);
  }

  BigUnsigned quotient;
  BigUnsigned remainder;

  for (size_t i = 0; i < left.digits_.size(); ++i) {
    remainder.digits_.push_back(left.digits_[i]);

    while (remainder.digits_.size() > 1 && remainder.digits_[0] == '0') {
      remainder.digits_.erase(remainder.digits_.begin());
    }

    unsigned count = 0;
    while ((remainder >= right)) {
      remainder = remainder - right;
      ++count;
    }

    quotient.digits_.push_back(count + '0');
  }

  while (quotient.digits_.size() > 1 && quotient.digits_[0] == '0') {
    quotient.digits_.erase(quotient.digits_.begin());
  }

  return quotient;
}

BigUnsigned BigUnsigned::operator%(const BigUnsigned& obj) const {
  if (obj == BigUnsigned()) {
    throw std::invalid_argument("Cannot divide by zero");
  } else if (*this < obj) {
    return *this; 
  }

  BigUnsigned remainder;

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

// Pre-incremento (++a)
BigUnsigned& BigUnsigned::operator++() {
  *this = *this + 1;
  return *this;
}

// Post-incremento (a++)
BigUnsigned BigUnsigned::operator++(int) {
  BigUnsigned temp = *this;
  ++(*this);  // Reutiliza el operador de pre-incremento
  return temp;
}

// Pre-decremento (--a)
BigUnsigned& BigUnsigned::operator--() {
  if (*this == BigUnsigned()) {
    throw std::underflow_error("BigUnsigned cannot be decremented below zero");
  }
  *this = *this - 1;  
  return *this;
}

// Post-decremento (a--)
BigUnsigned BigUnsigned::operator--(int) {
  BigUnsigned temp = *this;
  --(*this); 
  return temp;
}






