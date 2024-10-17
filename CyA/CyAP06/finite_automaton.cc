/**
 * @file finite_automaton.cc
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include "finite_automaton.h"

NFA::NFA(const Alfabeto& alphabet, const std::set<int>& states, const TrFunc& transitions_nfa,
const int initial_state, const std::set<int>& accepting_states) : alphabet_(alphabet), states_(states), transit_(transitions_nfa), 
initial_state_(initial_state), accepting_states_(accepting_states) {}

Alfabeto NFA::GetAlphabet() const {
  return alphabet_;
}

std::set<int> NFA::GetStates() const {
  return states_;
}

int NFA::GetNumberOfStates() const {
  return states_.size();
}

int NFA::GetInitialState() const {
  return initial_state_;
}

std::set<int> NFA::GetAcceptingStates() const {
  return accepting_states_;
}

TrFunc NFA::GetTr() const{
  return transit_;
}

std::ostream& operator<<(std::ostream& os, const NFA& obj){
  os << "\n\033[34m" << "=== FINITE AUTOMATON DESCRIPTION ===\n" << "\033[0m";  
  os << "\033[32m" << "Alphabet: " << "\033[0m" << obj.GetAlphabet() << "\n";
  os << "\033[32m" << "Inital state: " << "\033[0m" << obj.GetInitialState() << "\n";
  os << "\033[32m" << "State's set: " << "\033[0m";
  os << "{";
  bool first_st = true;  
  for (const int st : obj.GetStates()) {
    if (!first_st) {
      os << ", "; 
    }
   os << st;
   first_st = false; 
  }
  os << "}\n";

  os << "\033[32m" << "Accepting states: " << "\033[0m";
  os << "{";
  bool first = true;  
  for (const int acc_st : obj.GetAcceptingStates()) {
    if (!first) {
      os << ", "; 
    }
   os << acc_st;
   first = false; 
  }
  os << "}\n";

  os << "\033[32m"<< "NFA Transition Function:" << "\033[0m" << "\n";
  os << obj.GetTr();
  os << "\033[34m" << "=== END OF DESCRIPTION ===\n" << "\033[0m";
  return os;
}

