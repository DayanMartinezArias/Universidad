#ifndef CONTEXT_H
#define CONTEXT_H

#include <unordered_map>
#include <stdexcept>
#include <string>
#include<iostream>

#include "variable.h"

class Context {
 public:
  void setVariable(const std::string& name, const Var& value) {
    variables[name] = value;
  }
  Var getVariable(const std::string& name) const {
    if (variables.find(name) == variables.end()) {
      throw std::runtime_error("Undefined variable: " + name);
    }
    return variables.at(name);
  }
 private:
  std::unordered_map<std::string, Var> variables; // Tabla de variables.
};

#endif