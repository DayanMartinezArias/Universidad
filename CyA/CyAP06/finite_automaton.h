/**
 * @file finite_automaton.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-10-16
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef AUTOM_H
#define AUTOM_H

#include <iostream>
#include <set>
#include <map>
#include <string>
#include <utility>
#include "alfabeto.h"
#include "Transitions.h"

class NFA {
  public:
   NFA() = default;
   NFA(const Alfabeto& alphabet, const std::set<int>& states, const TrFunc& transitions_nfa,
   const int initial_state, const std::set<int>& accepting_states);
   Alfabeto GetAlphabet() const;
   std::set<int> GetStates() const;
   int GetNumberOfStates() const;
   int GetInitialState() const;
   std::set<int> GetAcceptingStates() const;
   friend std::ostream& operator<<(std::ostream& os, const NFA& obj);

  private:
   Alfabeto alphabet_; // Alphabet
   std::set<int> states_; // Set of states of the automaton
   TrFunc transit_; // Transition Function
   int initial_state_; // Initial state
   std::set<int> accepting_states_; // Set of all the accepting states
};

#endif