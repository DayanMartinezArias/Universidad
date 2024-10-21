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
 * Contiene la definición de la clase autómata
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
#include "cadena.h"

class NFA {
  public:
   NFA() = default;
   NFA(const Alfabeto& alphabet, const std::set<int>& states, const TrFunc& transitions_nfa,
   const int initial_state, const std::set<int>& accepting_states);
   NFA(const NFA& obj);
   Alfabeto GetAlphabet() const;
   std::set<int> GetStates() const;
   int GetNumberOfStates() const;
   int GetInitialState() const;
   std::set<int> GetAcceptingStates() const;
   TrFunc GetTr() const;
   bool ReadString(const Cadena& string) const;
   std::set<int> EpsClosure(const int state) const;
   std::set<int> EpsClosureSet(const std::set<int>& set_of_states) const;
   bool IsDFA() const;
   NFA GetDFA() const;
   bool InvalidNFA() const;
   friend std::ostream& operator<<(std::ostream& os, const NFA& obj);
   bool operator==(const NFA& obj) const;
   NFA& operator=(const NFA& obj);

  private:
   Alfabeto alphabet_; // Alphabet
   std::set<int> states_; // Set of states of the automaton
   TrFunc transit_; // Transition Function
   int initial_state_; // Initial state
   std::set<int> accepting_states_; // Set of all the accepting states
};

#endif
