#include "Production.h"

Production::Production(const char symbol, const std::string& secuence) {
  production_.first = symbol;
  production_.second = secuence;
}

std::pair<char, std::string> Production::GetProduction() const {
  return production_;
}

std::ostream& operator<<(std::ostream& os, const Production& obj) {
  os << obj.GetProduction().first << "-->" << obj.GetProduction().second;
  return os;
}
