#include "finite_automaton.h"
#include "alfabeto.h"
#include "Transitions.h"

int main () {
  Alfabeto alphabet;
  alphabet.InsertarSimbolo('0');
  alphabet.InsertarSimbolo('1');
  std::set<int> states = {0, 1};
  std::set<int> accepting_state{1};
  int initial_state{0};
  std::multimap<std::pair<char, int>, int> transitions_nfa;
  transitions_nfa.insert({{'0', 0}, 1});
  transitions_nfa.insert({{'1', 0}, 1});
  transitions_nfa.insert({{'0', 1}, 0});
  transitions_nfa.insert({{'1', 1}, 0});
  TrFunc tr(transitions_nfa);
  NFA autom(alphabet, states, transitions_nfa, initial_state, accepting_state);
  std::cout << autom << std::endl;
}