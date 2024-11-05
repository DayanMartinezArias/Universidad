#include "Production.h"
#include "grammar.h"
#include <iostream>
#include <fstream>

constexpr int kCorrectParams{3};

void Usage() {
std::cout << "Usage : ./Grammar2CNF input.gra output.gra" << std::endl;
std::cout << "Try: Grammar2CNF --help for more information" << std::endl;
}

bool CheckParams(int argc){
  if (argc != kCorrectParams) {
    std::cerr << "Incorrect number of parameters" << std::endl;
    std::cerr << "You must introduce exactly " << kCorrectParams << " including the program name" << std::endl;
    std::cerr << argc << " params were introduuced" << std::endl;
    return false;
  }
  return true;
}

void help() {
  std::cout << "===================================================" << std::endl;
  std::cout << "               CFG to CNF Converter                 " << std::endl;
  std::cout << "===================================================" << std::endl;
  std::cout << "Usage:" << std::endl;
  std::cout << "  ./Grammar2CNF input.gra output.gra" << std::endl;
  std::cout << "Description:" << std::endl;
  std::cout << "  Converts a Context-Free Grammar (CFG) to Chomsky Normal Form (CNF)." << std::endl;
  std::cout << "Parameters:" << std::endl;
  std::cout << "  input.gra  : The input file containing the CFG." << std::endl;
  std::cout << "  output.gra : The output file to save the CNF." << std::endl;
  std::cout << "Input File Format:" << std::endl;
  std::cout << "  1. First line: Number of terminals (n)." << std::endl;
  std::cout << "  2. Next n lines: Each line contains one terminal symbol." << std::endl;
  std::cout << "  3. Next line: Number of non-terminals (m)." << std::endl;
  std::cout << "  4. Next m lines: Each line contains one non-terminal symbol." << std::endl;
  std::cout << "  5. Next line: Number of productions (p)." << std::endl;
  std::cout << "  6. Next p lines: Each line contains one production in the form A-->B1...Bx." << std::endl;
  std::cout << "Examples:" << std::endl;
  std::cout << "  ./Grammar2CNF example_input.gra example_output.gra" << std::endl;
  std::cout << "===================================================" << std::endl;
}


int main (int argc, char* argv[]) {
  if (argc == 1) {
    Usage();
    return 0;
  }
  if (argc == 2) {
    std::string command(argv[1]);
    if (command == "--help") {
      help();
      return 0;
    }
  }
  if(!CheckParams(argc)) {
    return 1;
  }
  std::string first_param(argv[1]);
  std::string second_param(argv[2]);
  std::ifstream input_file(first_param);
  std::ofstream output_file(second_param);
  if (!input_file.is_open() || !output_file.is_open()) {
    std::cerr << "there was a problem opening the files introduced" << std::endl;
    return 1;
  }
  Grammar context_free_grammar;
  context_free_grammar.Read(input_file);
  Grammar chomsky_normal_form_grammar = context_free_grammar.ChomskyNormalForm();
  output_file << chomsky_normal_form_grammar;
  input_file.close();
  output_file.close();
  return 0;
}