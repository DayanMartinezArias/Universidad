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
  void PrintResults() const;
 private:
  bool IsMain;
  std::string header_;
  std::vector<Comment> comments_;
  std::vector<Loop> loops_;
  std::vector<Variable> vars_;
};

#endif