/**  Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Computabilidad y Algoritmia
 * Curso: 2º
 * Práctica 2: Autómatas finitos
 * Autor: Dayán Martínez Arias
 * Correo: alu0101644561@ull.edu.es
 * Fecha: 25/09/24
 * Archivo finite_autom_p06.cc
 * Main para la clase de autómatas finitos
 *
 */

#include <fstream>
#include <string>
#include <sstream>
#include <cstdlib>
#include "finite_automaton.h"
#include "alfabeto.h"
#include "Transitions.h"

constexpr int kCorrectParam{3};

/**
 * @brief Processes the input file in order to create a finite automaton
 * 
 * @param input 
 * @return NFA 
 */
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
        if (!alphabet.ExisteSimbolo(symbol) && symbol != kVacia) {
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

/**
 * @brief Processes the string's files
 * 
 * @param input_string 
 * @param output_file 
 * @param result 
 */
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

/**
 * @brief Offers a small text on how to use the program
 * 
 * @param argv 
 */
void help(char* argv[]) {
  std::cout << "Execute: " << argv[0] << " automata.fa " << "input.txt" << std::endl;
  std::cout << "automata.fa: Information about the automata." << std::endl;
  std::cout << "input.txt: Information about the input strings." << std::endl;
}

/**
 * @brief Checks if the amount of parameters introduced is correct
 * 
 * @param argc 
 * @return true 
 * @return false 
 */
bool CheckParams(const int argc) {
  if (argc > kCorrectParam) {
    std::cerr << "Inavlid number of parameters" << "\n";
    std::cerr << argc << " params were introduced" << "\n";
    std::cerr << "Please, introduce as maximum as " << kCorrectParam << " params" << "\n";
    return false;
  }
  return true;
}

/**
 * @brief Main function
 * 
 * @param argc 
 * @param argv 
 * @return int 
 */
int main (int argc, char* argv[]) {
  std::string f_command(argv[1]);
  if (f_command == "--help") {
    help(argv);
    return 0;
  }
  if (argc == 1) {
    help(argv);
    return 0;
  }
  if(!CheckParams(argc)) {
    return 1;
  }
  std::ifstream input_autom(argv[1]);
  std::ifstream input_string(argv[2]);
  std::ofstream output_file("output.txt");
  if (!input_autom.is_open()) {
    std::cerr << "ERROR: Automata file couldn't be opened" << "\n";
    return 1;
  }

  if (!input_string.is_open() && argc == 3) {
    std::cerr << "ERROR: String file couldn't be opened" << "\n";
    return 1;
  }
  NFA result = ProcessAutom(input_autom);
  if (result.InvalidNFA()) {
    return 1;
  }
  if (argc == 2) {
    input_autom.close();
    std::cerr << result << std::endl;
    return 0;
  }
  ProcessStrings(input_string, output_file, result);
  input_autom.close();
  input_string.close();
  return 0;
}