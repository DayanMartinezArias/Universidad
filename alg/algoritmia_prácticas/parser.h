#ifndef PARSER_H
#define PARSER_H

#include <stdexcept>
#include "lexer.h"
#include "AST.h"

/*
STMT -> PRINT EXPR END | ID ASSIGN EXPR END
EXPR -> NUMBER OP NUMBER | STRING "+" STRING | STRING
*/

class Parser {
 public:
  explicit Parser(const std::vector<token>& tokens) : tokens(tokens) {}
  void parse();
 private:
  //private methods
  ASTNode* ParseStatement();
  Expressions* ParseExpression();
  bool Match(const Type& type);
  bool Check(const Type& type);
  void Expected(const Type& type, const std::string& message);
  token Previous();
  // private member
  std::vector<token> tokens;
  size_t position = 0;
};

void Parser::parse() {
  Context context;
  while (position < tokens.size()) {
    ASTNode* statement = ParseStatement();
    statement->run(context);
    delete statement; 
  }
}

ASTNode* Parser::ParseStatement() {
  if (Match(Type::PRINT)) {
   Expressions* expr = ParseExpression();
    Expected(Type::END, "Missing semicolon after statement");
    return new Print(expr);
  } else if (Match(Type::ID)) {
    std::string name = Previous().value;
    Expected(Type::ASSIGN, "Missing assing operator in expression");
    Expressions* expr = ParseExpression();
    Expected(Type::END, "Missing semicolon after statement");
    return new Assigment(name, expr);
  }
}

Expressions* Parser::ParseExpression() {
  if (Match(Type::NUMBER)) {
    int left = std::stoi(Previous().value);
    Expected(Type::OP, "Missing operand in expression");
    std::string operand = Previous().value;
    Expected(Type::NUMBER, "Missing or incorrect value in expression after an operand");
    int right = std::stoi(Previous().value);
    return new BinaryExpr(new Numbers(left), new Numbers(right), operand);
  } else if (Match(Type::STRING)) {
    std::string left = Previous().value;
    if (Match(Type::OP)) {
      std::string operand = Previous().value;
      if(operand != "+") {
        throw std::runtime_error("Invalid operand");
      }
      Expected(Type::STRING, "Missing or incorrect value in expression after an operand");
      std::string right = Previous().value;
      return new BinaryExpr(new Text(left), new Text(right), operand);
    } else {
      return new Text(Previous().value);
    }
  } else {
    throw std::runtime_error("Expected an expression");
  }
}

bool Parser::Match(const Type& type) {
  if (Check(type)) {
    position++;
    return true;
  } else {
    return false;
  }
}

bool Parser::Check(const Type& type) {
  return position < tokens.size() && tokens[position].token_type == type;
}

void Parser::Expected(const Type& type, const std::string& message) {
  if (!Match(type)) {
    throw std::runtime_error(message);
  }
}

token Parser::Previous() {
  return tokens[position - 1];
}

#endif