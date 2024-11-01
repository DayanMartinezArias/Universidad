#include "Production.h"

Production::Production(const char symbol, const std::string& secuence) {
  production_.first = symbol;
  production_.second = secuence;
}

std::pair<char, std::string> Production::GetProduction() const {
  return production_;
}

bool Production::operator<(const Production& other) const {
  if (GetProduction().first != other.GetProduction().first) {
    return GetProduction().first  < other.GetProduction().first;
  }
  return GetProduction().second < other.GetProduction().second;
}

char Production::GetSymbol() const {
  return production_.first;
}

std::string Production::GetSecuence() const {
  return production_.second;
}

std::ostream& operator<<(std::ostream& os, const Production& obj) {
  os << obj.GetProduction().first << "-->" << obj.GetProduction().second;
  return os;
}
