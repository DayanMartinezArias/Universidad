#ifndef BIGINT_H
#define BIGINT_H

#include "BigUnsigned.h"

class BigInteger {
 public:
  BigInteger(int n = 0);
  BigInteger(const BigUnsigned&);

  BigInteger& operator-();
  bool operator==(const BigInteger& obj) const;
  friend bool operator<(const BigInteger& left, const BigInteger& right);

  friend std::ostream& operator<<(std::ostream& os, const BigInteger& obj);
  friend std::istream& operator>>(std::istream& is, BigInteger& obj);

  friend BigInteger operator+(const BigInteger& left, const BigInteger& right);
  BigInteger operator-(const BigInteger& obj) const;
  BigInteger operator*(const BigInteger& obj) const;
  friend BigInteger operator/(const BigInteger& left, const BigInteger& right);
  BigInteger operator%(const BigInteger& obj) const;

  BigInteger& operator++(); // Pre-incremento
  BigInteger operator++(int a); // Post-incremento
  BigInteger& operator--(); // Pre-decremento
  BigInteger operator--(int b); // Post-decremento

 private:
  BigUnsigned value_;
  bool sign_; // true positive, false negative
};

#endif