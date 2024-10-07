/**
 * @file Loops.h
 * @author Martínez Arias, Dayán alu0101644561
 * @brief Implementation of the Loops class, wich defines a loop
 * @version 0.1
 * @date 2024-10-07
 */

#include "Loops.h"

/**
 * @brief Construct a new Loop:: Loop object
 * 
 * @param type 
 * @param line 
 */
Loop::Loop(LoopTypes type, int line) : type_(type), line_(line) {}

/**
 * @brief Returns the type of loop
 * 
 * @return LoopTypes 
 */
LoopTypes Loop::GetType() const {
    return type_;
}

/**
 * @brief Returns the line
 * 
 * @return int 
 */
int Loop::GetLine() const {
    return line_;
}

/**
 * @brief Converts from enum to string
 * 
 * @param type 
 * @return std::string 
 */
std::string LoopTypeToString(LoopTypes type) {
  switch (type) {
    case LoopTypes::For: 
      return "for";
    case LoopTypes::While: 
      return "while";
    default: 
      return "unknown";
  }
}

/**
 * @brief Prints the loop
 * 
 * @param os 
 * @param loop 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Loop& loop) {
  os << "[ Line " << loop.GetLine() << "] LOOP: " << LoopTypeToString(loop.GetType());
  return os;
}

