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


BigUnsigned::BigUnsigned(unsigned number) {
  std::vector<unsigned char> digits = ConvertIntToChar(number);
  for (const unsigned char& digit: digits) {
    IsDigit(digit);
    digits_.emplace_back(digit);
  }
}

// Constructor that accepts an array of chars
BigUnsigned::BigUnsigned(const unsigned char* digits) {
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

// Overload of the << operaotr
std::ostream& operator<<(std::ostream& os, const BigUnsigned& obj) {
  for (const unsigned char& digit : obj.digits_) {
    os << digit;
  }
  return os;
}

// Overload of the operator >>
std::istream& operator>>(std::istream& is, BigUnsigned& obj) {
  obj.digits_.clear();
  std::string number_line;
  is >> number_line;
  for (const char& digit : number_line) {
    IsDigit(static_cast<unsigned char>(digit));
    obj.digits_.emplace_back(static_cast<unsigned char>(digit));
  }
  return is;
}

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

bool operator<(const BigUnsigned& left, const BigUnsigned& right) {
  if (left.digits_.size() < right.digits_.size()) {
    return true;
  } else if (left.digits_.size() == right.digits_.size()) {
    for (size_t i{0}; i < left.digits_.size(); ++i) {
      if (left.digits_[i] > right.digits_[i]) {
        return false;
      }
    }
    if (left == right) {
      return false;
    } else {
      return true;
    }
  } else {
    return false;
  }
}

// modulo lo que pones
// resultado el nuevo acarreo
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