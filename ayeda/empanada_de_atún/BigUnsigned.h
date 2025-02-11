#ifndef BIG_UNSIGNED
#define BIG_UNSIGNED

#include <vector>
#include <iostream>

class BigUnsigned {
 public:
  BigUnsigned(unsigned number = 0);
  BigUnsigned(const unsigned char* digits);
  BigUnsigned(const BigUnsigned& obj);

  BigUnsigned& operator=(const BigUnsigned& obj);

  friend std::ostream& operator<<(std::ostream& os, const BigUnsigned& obj);
  friend std::istream& operator>>(std::istream& is, BigUnsigned& obj);

  bool operator==(const BigUnsigned& obj) const;
  friend bool operator<(const BigUnsigned& left, const BigUnsigned& right);
  bool operator>=(const BigUnsigned& other) const;


  BigUnsigned& operator++(); // Pre-incremento
  BigUnsigned operator++(int a); // Post-incremento
  BigUnsigned& operator--(); // Pre-decremento
  BigUnsigned operator--(int b); // Post-decremento

  friend BigUnsigned operator+(const BigUnsigned& left, const BigUnsigned& right);
  BigUnsigned operator-(const BigUnsigned& obj) const;
  BigUnsigned operator*(const BigUnsigned& obj) const;
  friend BigUnsigned operator/(const BigUnsigned& left, const BigUnsigned& right);
  BigUnsigned operator%(const BigUnsigned& obj) const;
 private:
  BigUnsigned ChangeLength(const BigUnsigned& left, const BigUnsigned& right) const;
  BigUnsigned AddZeros(const BigUnsigned& number, int& sz) const;

  std::vector<unsigned char> digits_;
};

#endif