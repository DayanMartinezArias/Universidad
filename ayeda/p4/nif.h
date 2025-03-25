#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <stdexcept>

class nif {
 public:
  nif() : numero(10000000 + rand() % 90000000) {}
  nif(long num) : numero(num) {
    if (num < 10000000 || num > 99999999) {
      throw std::invalid_argument("NIF debe tener 8 dígitos");
    }
  }
  operator long() const { return numero; }
  bool operator==(const nif& other) const { return numero == other.numero; }
  bool operator!=(const nif& other) const { return !(*this == other); }
 private:
  long numero; 
};