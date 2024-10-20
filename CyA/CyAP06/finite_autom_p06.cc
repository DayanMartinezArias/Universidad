#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "finite_automaton.h"
#include "alfabeto.h"
#include "Transitions.h"

NFA ProcessAutom(std::ifstream& input) {
  int line_counter{1};
  int number_of_states;
  int init_state;
  std::set<int> states;
  std::set<int> accepting_states;
  std::string line;
  Alfabeto alphabet;
  std::multimap<std::pair<char, int>, int> transitions;
  std::set<int> check_tr;

  NFA invalid;

  while (std::getline(input, line)) {
    if (line_counter == 1) {
      for (const char symbol : line) {
        if (symbol == kVacia) {
          std::cerr << "ERROR: Invalid symbol for alphabet: " << kVacia << "\n";
          return invalid;
        }
        alphabet.InsertarSimbolo(symbol);
      }
    }
    else if (line_counter == 2) {
      number_of_states = std::stoi(line);
    }
    else if (line_counter == 3) {
      init_state = std::stoi(line);
    } else {
      int state = std::stoi(line.substr(0, 1));
      states.insert(state); 
    
      int is_accepting = std::stoi(line.substr(2, 1));
      if (is_accepting == 1) {
        accepting_states.insert(state);
      }
     int number_t = std::stoi(line.substr(4, 1)); 
     std::string transitions_info = line.substr(6); 
      if (transitions_info.size() != (number_t * 4) - 1) {
       std::cerr << "Error: At line " << line_counter << " from input file, the number of transitions specified doesn't match the actual number of transitions." << "\n";
       return invalid;
      }
      for (int i = 0; i < number_t; ++i) {
        // Cada transición ocupa 4 caracteres
        char symbol = transitions_info[i * 4];   
        if (!alphabet.ExisteSimbolo(symbol)) {
          std::cerr << "ERROR: Invalid transition, undefined symbol: " << symbol << "\n";
        }
        int dest_state = std::stoi(transitions_info.substr(i * 4 + 2, 2)); 
        check_tr.insert(dest_state);
        transitions.insert({{symbol, state}, dest_state});
      }
    }
    line_counter++;
  }
  if (states.size() != number_of_states) {
    std::cerr << "ERROR: Specified number of states and actual number of states doesn't match" << "\n";
    return invalid;
  } 
  for (const int dest_state : check_tr) {
    if (states.find(dest_state) == states.end()) {
      std::cerr << "ERROR: Invalid transitable state at line: " << line_counter << "\n";
      return invalid;
    }
  }
  NFA created_nfa(alphabet, states, transitions, init_state, accepting_states);
  return created_nfa;
}

void ProcessStrings(std::ifstream& input_string, std::ofstream& output_file, const NFA& result) {
  std::string line;
  while (std::getline(input_string, line)) {
    if(result.ReadString(line)) {
      output_file << line << " --Accepted" << "\n";
    } else {
      output_file << line << " --Rejected" << "\n";
    }
  }
}

int main (int argc, char* argv[]) {
  if (argv[1] == "--help") {

  }
  std::ifstream input_autom(argv[1]);
  std::ifstream input_string(argv[2]);
  std::ofstream output_file("output.txt");
  NFA result = ProcessAutom(input_autom);
  if (result.InvalidNFA()) {
    return 1;
  }
  ProcessStrings(input_string, output_file, result);
  input_autom.close();
  input_string.close();
  return 0;
}