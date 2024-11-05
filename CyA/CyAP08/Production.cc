#include "Production.h"

/**
 * @brief Construct a new Production:: Production object
 * 
 * @param symbol 
 * @param secuence 
 */
Production::Production(const char symbol, const std::string& secuence) {
  production_.first = symbol;
  production_.second = secuence;
}

/**
 * @brief Returns an std::pair containing each side of the production
 * 
 * @return std::pair<char, std::string> 
 */
std::pair<char, std::string> Production::GetProduction() const {
  return production_;
}

/**
 * @brief Orders the production
 * 
 * @param other 
 * @return true 
 * @return false 
 */
bool Production::operator<(const Production& other) const {
  if (GetProduction().first != other.GetProduction().first) {
    return GetProduction().first  < other.GetProduction().first;
  }
  return GetProduction().second < other.GetProduction().second;
}

/**
 * @brief Returns the non terminal symbol of the production
 * 
 * @return char 
 */
char Production::GetSymbol() const {
  return production_.first;
}

/**
 * @brief Returns the secuence of the production
 * 
 * @return std::string 
 */
std::string Production::GetSecuence() const {
  return production_.second;
}

/**
 * @brief Prints the production
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Production& obj) {
  os << obj.GetProduction().first << "-->" << obj.GetProduction().second;
  return os;
}
