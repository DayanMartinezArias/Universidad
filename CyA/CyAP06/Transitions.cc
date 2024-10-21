/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Autómatas finitos
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo Transition.cc
 * Contiene la implementación de la clase transición
 *
 */

#include "Transitions.h"

/**
 * @brief 
 * 
 * @param state 
 * @param symbol 
 * @return std::set<int> 
 */
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

/**
 * @brief 
 * 
 * @param state 
 * @param symbol 
 * @return int 
 */
int TrFunc::GetNextDFA(const int state, const char symbol) const {
  if (is_dfa_) {
    auto it = transitions_dfa_.find({symbol, state});
    if (it != transitions_dfa_.end()) {
      return it->second; 
    }
  }
  throw std::runtime_error("Invalid DFA transition");
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool TrFunc::IsDFA() const{
  return is_dfa_;
}

/**
 * @brief 
 * 
 * @return std::multimap<std::pair<char, int>, int> 
 */
std::multimap<std::pair<char, int>, int> TrFunc::GetFunctionNFA() const {
  return transitions_nfa_;
}

/**
 * @brief 
 * 
 * @return std::map<std::pair<char, int>, int> 
 */
std::map<std::pair<char, int>, int> TrFunc::GetFunctionDFA() const {
  return transitions_dfa_;
}

/**
 * @brief 
 * 
 * @param obj 
 * @return true 
 * @return false 
 */
bool TrFunc::operator==(const TrFunc& obj) const {
  if (is_dfa_ != obj.is_dfa_) {
    return false;
  }
  if (IsDFA()) {
    return GetFunctionDFA() == obj.GetFunctionDFA();
  } else {
    return GetFunctionNFA() == obj.GetFunctionNFA();
  }
}

/**
 * @brief 
 * 
 * @return true 
 * @return false 
 */
bool TrFunc::Empty() const {
  return transitions_nfa_.empty();
}

/**
 * @brief 
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
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