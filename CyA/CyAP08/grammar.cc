#include "grammar.h"

#include <algorithm>
#include <cctype>


bool isNumber(const std::string& number) {
  return !number.empty() && std::all_of(number.begin(), number.end(), ::isdigit);
}


// ESPACIOS
// repe
// que se acabe el fichero
bool Grammar::Read(std::ifstream& input_file) {
  bool valid_file{true};  // Cambiado a true para confirmar que es válido a menos que ocurra un error
  int line_counter{1};
  int number_of_terminals{0};
  int number_of_non_terminals{0};
  
  Alfabeto alphabet;

  std::string line;
  while (std::getline(input_file, line)) {
    if (line.empty()) {
      continue;
    }

    if (line_counter == 1) {
      // Primer línea: número de terminales
      if (!isNumber(line)) {
        std::cerr << "On line 1: Invalid value for the number of terminal symbols" << std::endl;
        valid_file = false;
        break;
      }
      number_of_terminals = std::stoi(line);
      if (number_of_terminals < 1) {
        std::cerr << "You must have at least 1 terminal symbol" << std::endl;
        valid_file = false;
        break;
      }
    } 
    else if (line_counter > 1 && line_counter <= number_of_terminals + 1) {
      // Línea de terminales
      if (line.length() > 1) {
        std::cerr << "Please, insert only symbols with 1 character on line " << line_counter << std::endl;
        valid_file = false;
        break;
      }
      char symbol = line[0];
      alphabet.InsertarSimbolo(symbol);
    }
    else if (line_counter == number_of_terminals + 2) {
      // Línea para el número de no terminales
      if (!isNumber(line)) {
         std::cerr << "On line " << line_counter << ": Invalid value for the number of non-terminal symbols" << std::endl;
         valid_file = false;
         break;
      }
      number_of_non_terminals = std::stoi(line);
      if (number_of_non_terminals < 1) {
        std::cerr << "You must have at least 1 non-terminal symbol" << std::endl;
        valid_file = false;
        break;
      }
    }

    // Incrementa el contador de línea después de procesar cada línea
    line_counter++;
  }
  // Imprimir el alfabeto solo si el archivo es válido
  if (valid_file) {
    std::cout << alphabet << std::endl;
  }
  
  return valid_file;
}
