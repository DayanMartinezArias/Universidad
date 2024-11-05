#ifndef PRODUCTION_H
#define PRODUCTION_H

#include <iostream>
#include <string>
#include <utility>

class Production {
public:
  Production(const char symbol, const std::string& secuence);
  std::pair<char, std::string> GetProduction() const;
  bool operator<(const Production& other) const;
  char GetSymbol() const;
  std::string GetSecuence() const;
  friend std::ostream& operator<<(std::ostream& os, const Production& obj);

private:
  std::pair<char, std::string> production_; // std::pair that stores each side of the production
};

#endif 
