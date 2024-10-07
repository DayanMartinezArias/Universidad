#include "Variable.h"

/**
 * @brief Construct a new Variable:: Variable object
 * 
 * @param type 
 * @param name 
 * @param line 
 * @param init 
 * @param init_val 
 */
Variable::Variable(const VarTypes& type, const std::string& name, int line, bool init, std::string init_val)
  : type_(type), name_(name), line_(line), initialized_(init), init_val_(init_val) {}

/**
 * @brief Returns the type of variable
 * 
 * @return VarTypes 
 */
VarTypes Variable::GetType() const {
    return type_;
}

/**
 * @brief Returns the name of the variable
 * 
 * @return std::string 
 */
std::string Variable::GetName() const {
    return name_;
}

/**
 * @brief Returns the line
 * 
 * @return int 
 */
int Variable::GetLine() const {
    return line_;
}

/**
 * @brief Returns whether the variable is initializzed or not
 * 
 * @return true 
 * @return false 
 */
bool Variable::IsInitialized() const {
    return initialized_;
}

/**
 * @brief Returns the initial value of a variable
 * 
 * @return std::string 
 */
std::string Variable::GetInitVal() const {
  return init_val_;
}

/**
 * @brief Converts an enum to a string
 * 
 * @param type 
 * @return std::string 
 */
std::string VarTypeToString(VarTypes type) {
  switch (type) {
    case VarTypes::Int: 
      return "INT";
    case VarTypes::Double: 
      return "DOUBLE";
    default: 
      return "unknown";
  }
}

/**
 * @brief Prints the variable
 * 
 * @param os 
 * @param var 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Variable& var) {
    os << "[ Line " << var.GetLine() << "] " << VarTypeToString(var.GetType()) << ": " << var.GetName();
    if (var.IsInitialized()) {
        os << " " << var.GetInitVal();
    }
    return os;
}
