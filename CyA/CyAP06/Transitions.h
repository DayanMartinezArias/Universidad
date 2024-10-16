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

class TrFunc {
 public:
  TrFunc() = default;
  TrFunc(const std::multimap<int, std::pair<char, int>>& transitions_nfa) : transitions_nfa_(transitions_nfa), is_dfa_(false) {};
  TrFunc(const std::map<char, int>& transitions_dfa) : transitions_dfa_(transitions_dfa), is_dfa_(true) {};
  std::set<int> GetNextNFA(const int state, const char symbol);
  int GetNextDFA(const int state, const char symbol);
  // friend std::ostream& operator<<(std::ostream& os, const TrFunc& obj);
 private:
  bool is_dfa_; // Checks if the transition function is associated with a DFA
  std::map<char, int> transitions_dfa_;
  std::multimap<int, std::pair<char, int>> transitions_nfa_;
};

#endif