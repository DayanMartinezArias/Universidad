#ifndef GRAMMAR_H
#define GRAMAR_H

#include <set>
#include <fstream>
#include "Production.h"
#include "alfabeto.h"

constexpr char kEmptyString{'&'};

class Grammar {
 public:
  Grammar() = default;
  bool Read(std::ifstream& input_file);
 private:
  bool ValidateProduction(const std::string& production, const Alfabeto& alphabet, const Alfabeto& non_t, char& left_side, std::string& right_side);
  bool CheckLines(std::ifstream& input_file, int& number_of_terminals, int& number_of_non_terminals, int& number_of_productions);

  Alfabeto alphabet;
  Alfabeto non_t;
  char start_symbol_;
  std::set<Production> productions_;
};

#endif