/**
 * @file Analizer.h
 * @author Martínez Arias, Dayánn alu0101644561
 * @brief Declaration of the Analizer class
 * @version 0.1
 * @date 2024-10-07
 */

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
  Analyzer(std::vector<std::string> vector_of_strings, std::string name);
  std::string GetName() const;
  void Analyze();
  void LoopSearch(const std::string line, std::smatch match, std::regex loop_regex, int line_number);
  void VarSearch(const std::string line, std::smatch match, std::regex var_regex, int line_number);
  void SingleCommentSearch(const std::string line, std::smatch match, std::regex single_line_comment_regex, int line_number);
  bool DetectMain(const std::string& line);
  int WhileAmount() const;
  int ForAmount() const;
  int DoubleAmount() const;
  int IntAmount() const;
  friend std::ostream& operator<<(std::ostream& os, const Analyzer& obj);
 private:
  std::vector<std::string> all_content_;
  std::string name_;
  bool is_main_;
  std::vector<Comment> comments_;
  std::vector<Loop> loops_;
  std::vector<Variable> vars_;
};

#endif