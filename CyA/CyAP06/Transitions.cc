/**
 * @file Transitions.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-17
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "Transitions.h"

std::set<int> TrFunc::GetNextNFA(const int state, const char symbol) const {
  std::set<int> possible_states;
  if (!is_dfa_) {
    auto range = transitions_nfa_.equal_range({symbol, state});
    for (auto it = range.first; it != range.second; ++it) {
      possible_states.insert(it->second);
    }
  }
   return possible_states;
}

int TrFunc::GetNextDFA(const int state, const char symbol) const {
  if (is_dfa_) {
    auto it = transitions_dfa_.find({symbol, state});
    if (it != transitions_dfa_.end()) {
      return it->second; 
    }
  }
  throw std::runtime_error("Invalid DFA transition");
}

std::ostream& operator<<(std::ostream& os, const TrFunc& obj) {
  if (obj.is_dfa_) {
    for (const auto& transition : obj.transitions_dfa_) {
      os << "State: " << transition.first.second << "Symbol: " << transition.first.first << " -> Next State: " << transition.second << '\n';
    }
  } else {
      for (const auto& transition : obj.transitions_nfa_) {
        os << "State: " << transition.first.second  << ", Symbol: " << transition.first.first  << " -> Next State: " << transition.second << '\n';
      }
  }
  return os;
}