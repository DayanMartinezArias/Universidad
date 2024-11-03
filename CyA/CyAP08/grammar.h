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
  friend std::ostream& operator<<(std::ostream& os, const Grammar& obj);
  Grammar ChomskyNormalForm() const;
 private:

  bool ValidateProduction(const std::string& production, const Alfabeto& alphabet, const Alfabeto& non_t, char& left_side, std::string& right_side) const;
  bool CheckLines(std::ifstream& input_file, int& number_of_terminals, int& number_of_non_terminals, int& number_of_productions) const;
  bool CheckNullProduction(const std::set<Production>& production_set, const char start_symbol) const;
  bool CheckUnitaryproductions(const std::set<Production>& production_set, const Alfabeto& non_t) const;
  bool CheckDefinedStartSymbol(std::set<Production> production_set, const char start_symbol) const;

  Alfabeto alphabet_;
  Alfabeto non_t_;
  char start_symbol_;
  std::set<Production> productions_;
};

#endif