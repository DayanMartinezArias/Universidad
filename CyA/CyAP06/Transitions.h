/**
 * @file Tr.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef TRANSITION_H
#define TRANSITION_H

#include <iostream>
#include <map>
#include <set>
#include "alfabeto.h"

class TrFunc {
 public:
  TrFunc() = default;
  TrFunc(const std::multimap<std::pair<char, int>, int>& transitions_nfa) : transitions_nfa_(transitions_nfa), is_dfa_(false) {};
  TrFunc(const std::map<std::pair<char, int>, int>& transitions_dfa) : transitions_dfa_(transitions_dfa), is_dfa_(true) {};
  std::set<int> GetNextNFA(const int state, const char symbol) const;
  std::multimap<std::pair<char, int>, int> GetFunctionNFA() const;
  std::map<std::pair<char, int>, int> GetFunctionDFA() const;
  int GetNextDFA(const int state, const char symbol) const;
  bool IsDFA() const;
  friend std::ostream& operator<<(std::ostream& os, const TrFunc& obj);
  bool operator==(const TrFunc& obj) const;
 private:
  bool is_dfa_; // Checks if the transition function is associated with a DFA
  std::map<std::pair<char, int>, int> transitions_dfa_;
  std::multimap<std::pair<char, int>, int> transitions_nfa_;
};

#endif