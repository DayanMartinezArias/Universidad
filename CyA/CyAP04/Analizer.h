#ifndef ANALZER_H
#define ANALYZER_H

#include <iostream>
#include <fstream>
#include <regex>
#include <vector>

#include "Comments.h"
#include "Loops.h"
#include "Variable.h"

class Variable;
class Loop;
class Comment;

class Analyzer {
 public:
  Analyzer() = default;
  Analyzer(std::string name);
  std::string GetName() const;
  void PrintResults() const;
 private:
  std::string name_;
  bool is_main_;
  std::vector<Comment> comments_;
  std::vector<Loop> loops_;
  std::vector<Variable> vars_;
};

#endif