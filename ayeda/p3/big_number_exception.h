#ifndef BIG_NUMBER_EXCEPTION_H
#define BIG_NUMBER_EXCEPTION_H

#include <exception>
#include <string>

class BigNumberException : public std::exception {
protected:
  std::string message;
public:
  explicit BigNumberException(const std::string& msg) : message(msg) {}

  virtual const char* what() const noexcept override {
    return message.c_str();
  }
};

class BigNumberBadDigit : public BigNumberException {
public:
  explicit BigNumberBadDigit(const std::string& input) : BigNumberException("Error: Invalid dígit for base -> " + input) {}
};

class BigNumberDivisionByZero : public BigNumberException {
public:
  BigNumberDivisionByZero() : BigNumberException("Error: Division by zero.") {}
};

#endif