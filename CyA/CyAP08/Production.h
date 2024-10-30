#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <iostream>
#include <string>
#include <utility>

class Production {
public:
  Production(const char symbol, const std::string& secuence);
  std::pair<char, std::string> GetProduction() const;

  // Declaración del operador << para la clase Production
  friend std::ostream& operator<<(std::ostream& os, const Production& obj);

private:
  std::pair<char, std::string> production_;
};

#endif // PRODUCTION_H
