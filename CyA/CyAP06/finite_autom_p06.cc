#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "finite_automaton.h"
#include "alfabeto.h"
#include "Transitions.h"

void ProcessAutom(std::ifstream& input) {
  int line_counter{1};
  int number_of_states;
  int init_state;
  std::set<int> accepting_states;
  std::string line;
  Alfabeto alphabet;
  while (std::getline(input, line)) {
    if (line_counter == 1) {
      for (const char symbol : line) {
        alphabet.InsertarSimbolo(symbol);
      }
    }
    else if (line_counter == 2) {
      number_of_states = std::stoi(line);
    }
    else if (line_counter == 3) {
      init_state = std::stoi(line);
    }
    std::multimap<std::pair<char, int>, int> transitions;
    line_counter++;
  }
}

int main (int argc, char* argv[]) {
  std::ifstream input_autom(argv[1]);
  std::ifstream input_string(argv[2]);
  ProcessAutom(input_autom);
  input_autom.close();
  input_autom.close();
}