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

std::ostream& operator<<(std::ostream& os, const NFA& obj){
  os << "\033[34m" << "FINITE AUTOMATON:\n" << "\033[0m";  
  os << "\033[32m" << "Alphabet: " << "\033[0m" << obj.GetAlphabet() << "\n";
  os << "\033[32m" << "Inital state: " << "\033[0m" << obj.GetInitialState() << "\n";

  return os;
}

