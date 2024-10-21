/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Autómatas finitos
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo finite_automaton.cc
 * Contiene la implementación de la clase autómata
 *
 */

#include <queue>
#include "finite_automaton.h"

/**
 * @brief Construct a new NFA::NFA object
 * 
 * @param alphabet 
 * @param states 
 * @param transitions_nfa 
 * @param initial_state 
 * @param accepting_states 
 */
NFA::NFA(const Alfabeto& alphabet, const std::set<int>& states, const TrFunc& transitions_nfa,
const int initial_state, const std::set<int>& accepting_states) : alphabet_(alphabet), states_(states), transit_(transitions_nfa), 
initial_state_(initial_state), accepting_states_(accepting_states) {}

/**
 * @brief Construct a new NFA::NFA object
 * 
 * @param obj 
 */
NFA::NFA(const NFA& obj) : alphabet_(obj.GetAlphabet()), states_(obj.GetStates()), transit_(obj.GetTr()),
initial_state_(obj.GetInitialState()), accepting_states_(obj.GetAcceptingStates()) {}

/**
 * @brief 
 * 
 * @return Alfabeto 
 */
Alfabeto NFA::GetAlphabet() const {
  return alphabet_;
}

/**
 * @brief 
 * 
 * @return std::set<int> 
 */
std::set<int> NFA::GetStates() const {
  return states_;
}

/**
 * @brief 
 * 
 * @return int 
 */
int NFA::GetNumberOfStates() const {
  return states_.size();
}

/**
 * @brief 
 * 
 * @return int 
 */
int NFA::GetInitialState() const {
  return initial_state_;
}

/**
 * @brief 
 * 
 * @return std::set<int> 
 */
std::set<int> NFA::GetAcceptingStates() const {
  return accepting_states_;
}

/**
 * @brief 
 * 
 * @return TrFunc 
 */
TrFunc NFA::GetTr() const {
  return transit_;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool NFA::IsDFA() const {
  return transit_.IsDFA();
}

/**
 * @brief 
 * 
 * @param state 
 * @return std::set<int> 
 */
std::set<int> NFA::EpsClosure(const int state) const {
  std::set<int> eps_cl{state};
  std::queue<int> states_to_process({state});
  while (!states_to_process.empty()) {
    int c_state{states_to_process.front()};
    states_to_process.pop();
    std::set<int> next = GetTr().GetNextNFA(c_state, kVacia);
    for (const int next_states : next) {
      if (eps_cl.find(next_states) == eps_cl.end()) {
        states_to_process.push(next_states);
        eps_cl.insert(next_states);
      }
    }
  }
  return eps_cl;
}

/**
 * @brief 
 * 
 * @param set_of_states 
 * @return std::set<int> 
 */
std::set<int> NFA::EpsClosureSet(const std::set<int>& set_of_states) const {
  std::set<int> eps_cl = set_of_states;
  for (const int state : set_of_states) {
   std::set<int> next_states = EpsClosure(state);
    eps_cl.insert(next_states.begin(), next_states.end());
    next_states.clear();
  }
  return eps_cl;
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool NFA::InvalidNFA() const {
  return alphabet_.Vacio() && states_.empty() && transit_.Empty() && accepting_states_.empty();
}

/**
 * @brief 
 * 
 * @param string 
 * @return true 
 * @return false 
 */
bool NFA::ReadString(const Cadena& string) const {
  std::set<int> current_state{EpsClosure(GetInitialState())};
  std::set<int> next_states;
  if (string.GetSecuencia() == "&") {
    std::set<int> eps_cl = EpsClosureSet(current_state);
    for (int states: eps_cl) {
    auto it = GetAcceptingStates().find(states);
    if (it != GetAcceptingStates().end()) {
      return true;
    }
  }
  return false;
  }

  for (const char symbol : string.GetSecuencia()) {
    for (const int states : current_state) {
      std::set<int> res = GetTr().GetNextNFA(states, symbol);
      std::set<int> eps_cl = EpsClosureSet(res);
      next_states.insert(eps_cl.begin(), eps_cl.end());   
    }
    current_state = next_states;
    next_states.clear();
  }
  for (int last_state : current_state) {
    auto it = GetAcceptingStates().find(last_state);
    if (it != GetAcceptingStates().end()) {
      return true;
    }
  }
  return false;
}

/**
 * @brief 
 * 
 * @param obj 
 * @return true 
 * @return false 
 */
bool NFA::operator==(const NFA& obj) const {
  return GetInitialState() == obj.GetInitialState() && GetAlphabet() == obj.GetAlphabet() && 
  GetStates() == obj.GetStates() && GetAcceptingStates() == obj.GetAcceptingStates() && GetTr() == obj.GetTr();
}

/**
 * @brief 
 * 
 * @param obj 
 * @return NFA& 
 */
NFA& NFA::operator=(const NFA& obj) {
  if(this == &obj) return *this;
  alphabet_ = obj.GetAlphabet();
  accepting_states_ = obj.GetAcceptingStates();
  initial_state_ = obj.GetInitialState();
  transit_ = obj.GetTr();
  states_ = obj.GetStates();
  return *this;
}

/**
 * @brief 
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
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

