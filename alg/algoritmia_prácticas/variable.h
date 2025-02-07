#ifndef VARIABLE_H
#define ARIABLE_H

#include <unordered_map>
#include <stdexcept>
#include <string>
#include<iostream>

enum class Val_Type {
  INT,
  STRING
};

class Var {
 public: 
  Var() = default;
  Var(const Val_Type& type, int val) : type_(type), numeric_val_(val) {}
  Var(const Val_Type& type, std::string val) : type_(type), text_(val) {}
  bool IsType(const Val_Type& val_type) {
    return type_ == val_type;
  }
  
  Var operator+(const Var& other) const; 

  std::string get_string() const {
    return text_;
  }
  int get_int() const {
    return numeric_val_;
  }

  friend std::ostream& operator<<(std::ostream& obj, const Var& variable);
 private:
  Val_Type type_;
  int numeric_val_;
  std::string text_;
};

Var Var::operator+(const Var& other) const {
  if (other.type_ == Val_Type::STRING) {
    return Var(Val_Type::STRING, text_ + other.text_);
  } else if (other.type_ == Val_Type::INT) {
    return Var(Val_Type::STRING, numeric_val_+ other.numeric_val_);
  }
}

std::ostream& operator<<(std::ostream& os, const Var& variable) {
  if (variable.type_ == Val_Type::STRING) {
    os << variable.text_;
    return os;
  } else if (variable.type_ == Val_Type::INT) {
    os << variable.numeric_val_;
    return os;
  }
}

#endif