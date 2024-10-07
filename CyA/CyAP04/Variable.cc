#include "Variable.h"

// Constructor
Variable::Variable(const VarTypes& type, const std::string& name, int line, bool init, std::string init_val)
  : type_(type), name_(name), line_(line), initialized_(init), init_val_(init_val) {}

// Getter para obtener el tipo de variable
VarTypes Variable::GetType() const {
    return type_;
}

// Getter para obtener el nombre de la variable
std::string Variable::GetName() const {
    return name_;
}

// Getter para obtener la línea donde se declara la variable
int Variable::GetLine() const {
    return line_;
}

// Getter para saber si la variable está inicializada o no
bool Variable::IsInitialized() const {
    return initialized_;
}

std::string Variable::GetInitVal() const {
  return init_val_;
}

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

// Sobrecarga del operador de inserción para imprimir la variable
std::ostream& operator<<(std::ostream& os, const Variable& var) {
    os << "[ Line " << var.GetLine() << "] " << VarTypeToString(var.GetType()) << ": " << var.GetName();
    if (var.IsInitialized()) {
        os << " " << var.GetInitVal();
    }
    return os;
}
