#ifndef GRAMMAR_H
#define GRAMAR_H

#include <set>
#include <fstream>
#include "Production.h"
#include "alfabeto.h"

class Grammar {
 public:
  Grammar() = default;
  bool Read(std::ifstream& input_file);
 private:
  Alfabeto alphabet;
  Alfabeto non_t;
  char start_symbol_;
  std::set<Production> productions_;
};

#endif