#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>
#include <iostream>

class Analyzer;

enum VarTypes {
  Int,
  Double
};

class Variable {
 public:
  // Constructor
  Variable(const VarTypes& type, const std::string& name, int line, bool init, std::string init_val = "%");

  // Getters
  VarTypes GetType() const;
  std::string GetName() const;
  int GetLine() const;
  bool IsInitialized() const;
  std::string GetInitVal() const;

  // Sobrecarga del operador de inserción para imprimir la información de la variable
  friend std::ostream& operator<<(std::ostream& os, const Variable& var);

 private:
  VarTypes type_;      // Tipo de dato (int o double)
  std::string name_;      // Nombre de la variable
  int line_;              // Línea donde se declara
  bool initialized_;      // Si está inicializada o no
  std::string init_val_;
};

#endif
