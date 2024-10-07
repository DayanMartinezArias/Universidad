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
 
  Variable(const VarTypes& type, const std::string& name, int line, bool init, std::string init_val = "%");


  VarTypes GetType() const;
  std::string GetName() const;
  int GetLine() const;
  bool IsInitialized() const;
  std::string GetInitVal() const;

 
  friend std::ostream& operator<<(std::ostream& os, const Variable& var);

 private:
  VarTypes type_;     
  std::string name_;     
  int line_;            
  bool initialized_;     
  std::string init_val_;
};

#endif
