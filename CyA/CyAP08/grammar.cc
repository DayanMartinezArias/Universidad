#include "grammar.h"

#include <algorithm>
#include <cctype>


bool isNumber(const std::string& number) {
  return !number.empty() && std::all_of(number.begin(), number.end(), ::isdigit);
}
// que sea más largo
bool CheckLines(std::ifstream& input_file, int& number_of_terminals, int& number_of_non_terminals, int& number_of_productions) {
  std::string line;
  int line_counter{1};
  while (std::getline(input_file, line)) {
    if (line.empty()) {
      std::cerr << "Please, do not leave blank lines inside the input file, the blank line was found at line: " << line_counter << std::endl;
      return false;
    }
    if (line_counter == 1 ) {
      if (!isNumber(line)) {
        std::cerr << "On line 1: Invalid value for the number of terminal symbols" << std::endl;
        return false;
      }
      number_of_terminals = std::stoi(line);
      if (number_of_terminals < 1) {
        std::cerr << "You must have at least 1 terminal symbol" << std::endl;
        return false;
      }
    } else if (line_counter == number_of_terminals + 2) {
      if (!isNumber(line)) {
        std::cerr << "On line " << line_counter << ": Invalid value for the number of terminal symbols" << std::endl;
        return false;
      }
      number_of_non_terminals = std::stoi(line);
      if (number_of_non_terminals < 1) {
        std::cerr << "You must have at least 1 terminal symbol" << std::endl;
        return false;
      }      
    } else if (line_counter == number_of_non_terminals + number_of_terminals + 3) {
      if (!isNumber(line)) {
        std::cerr << "On line " << line_counter << ": Invalid value for the number of terminal symbols" << std::endl;
        return false;
      }
      number_of_productions = std::stoi(line);
      if (number_of_productions < 1) {
        std::cerr << "You must have at least 1 terminal symbol" << std::endl;
        return false;
      }      
    }
    line_counter++;
  }
  int total_lines{number_of_non_terminals + number_of_terminals + number_of_productions};
  if ((total_lines + 3) != (line_counter - 1)) {
    std::cerr << "There were supposed to be " << total_lines + 3 << " lines on the input file, you have " << line_counter - 1 << " lines, please, correct your input" << std::endl;
  }
  return true;
}

// ESPACIOS
// repeticiones
// que se acabe el fichero
bool Grammar::Read(std::ifstream& input_file) {
  int number_of_terminals{0};
  int number_of_non_terminals{0};
  int number_of_productions{0};
  int line_counter{1};
  
  Alfabeto alphabet;
  Alfabeto non_t;

  if (!CheckLines(input_file, number_of_terminals, number_of_non_terminals, number_of_productions)) {
    return false;
  }
  
  std::string line;
  while (getline(input_file, line)) {
    if (line_counter < 1 && line_counter <= number_of_terminals + 1) {
      if (line.length() > 1) {
        std::cerr << "Please, insert only symbols with 1 character on line " << line_counter << std::endl;
        return false;
      }
      char symbol = line[0];
      alphabet.InsertarSimbolo(symbol);
    } else if (line_counter > (number_of_terminals + 2) && line_counter <= number_of_non_terminals + number_of_terminals + 2) {
      if (line.length() > 1) {
        std::cerr << "Please, insert only symbols with 1 character on line " << line_counter << std::endl;
        return false;
      }
        char symbol = line[0];
        non_t.InsertarSimbolo(symbol);
      }  
    if (alphabet.GetCardinal() != number_of_terminals) {
      std::cerr << "The number terminals doesn't match the specified number" << std::endl;
      return false;
    }
    if (non_t.GetCardinal() != number_of_non_terminals) {
      std::cerr << "The number non-terminals doesn't match the specified number" << std::endl;
      return false;
    }
    std::cout << alphabet << std::endl;
    std::cout << non_t << std::endl;
  }
  return true;   
}
