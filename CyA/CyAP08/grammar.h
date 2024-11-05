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
  bool Empty() const;
  void PrintInfo() const;
 private:

  bool ValidateProduction(const std::string& production, const Alfabeto& alphabet, const Alfabeto& non_t, char& left_side, std::string& right_side) const;
  bool CheckLines(std::ifstream& input_file, int& number_of_terminals, int& number_of_non_terminals, int& number_of_productions) const;
  bool CheckNullProduction(const std::set<Production>& production_set, const char start_symbol) const;
  bool CheckUnitaryproductions(const std::set<Production>& production_set, const Alfabeto& non_t) const;
  bool CheckDefinedStartSymbol(std::set<Production> production_set, const char start_symbol) const;
  char GenerateNewNonTerminal(const Alfabeto& new_non_t) const;

  Alfabeto alphabet_;  // Alphabet of the class
  Alfabeto non_t_;  // Set of non terminal
  char start_symbol_; // The start symbol
  std::set<Production> productions_; // Set of all productions
};

#endif