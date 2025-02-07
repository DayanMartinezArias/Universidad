#ifndef AST_H
#define AST_H

#include <iostream>
#include <unordered_map>

#include "context.h"

/*
STMT -> PRINT EXPR END | ID ASSIGN EXPR END
EXPR -> NUMBER OP NUMBER | STRING "+" STRING | STRING
*/

// Clase Base del Árbol de análisis sintáctico
class ASTNode {
 public:
  virtual ~ASTNode() = default;
  virtual void run(Context& context) = 0;
};

// Clase para las declaraciones, tipo: screen 5 + 9; 
class Statements : public ASTNode{};

// Las expresiones son las diferentes partes que componen una declaración
class Expressions : public ASTNode {
  public:
   virtual Var get_value() = 0; // Cada expresión se subdivide en diferente partes con valores cada una
};

class Print : public Statements {
  public: 
   Print(Expressions* expr) : expr_(expr) {}
   void run(Context& context) override {
     std::cout << expr_->get_value() << std::endl;
   }
  private:
   Expressions* expr_;
};

class Assigment : public Statements {
 public:
  Assigment(const std::string& var_name, Expressions* expr) : var_name_(var_name), expr_(expr) {}
  void run (Context& context) override {
    context.setVariable(var_name_, expr_->get_value());
  }
 private:
  std::string var_name_;
  Expressions* expr_;
};

class Numbers : public Expressions {
 public:
  explicit Numbers(int val) : val_(val) {}
  Var get_value() override {
    return Var(Val_Type::INT, val_);
  }
  void run(Context& context) {}
 private:
  int val_;
};

class Text : public Expressions {
 public:
  explicit Text(std::string val) : val_(val) {}
  Var get_value() override {
    return Var(Val_Type::STRING, val_);
  }
  void run(Context& context) {}
 private:
  std::string val_;
};

class BinaryExpr : public Expressions{
 public:
  BinaryExpr(Expressions* right, Expressions* left, std::string operand) 
    : right_(right), left_(left), operand_(operand) {}

  Var get_value() override {
    if (operand_ == "+") return left_->get_value() + right_->get_value();
    else if (operand_ == "-") return Var(Val_Type::INT, left_->get_value().get_int() - right_->get_value().get_int());
    else if (operand_ == "*") return Var(Val_Type::INT, left_->get_value().get_int() * right_->get_value().get_int());
    else if (operand_ == "/") return Var(Val_Type::INT, left_->get_value().get_int() / right_->get_value().get_int());
  }
  void run(Context& context) {}
  
 private:
  Expressions* right_;
  Expressions* left_;
  std::string operand_;
};

#endif


