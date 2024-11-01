#include "grammar.h"

#include <algorithm>
#include <cctype>
// chequear prod vacías
// chquear prd unit

bool isNumber(const std::string& number) {
  return !number.empty() && std::all_of(number.begin(), number.end(), ::isdigit);
}

bool Grammar::CheckLines(std::ifstream& input_file, int& number_of_terminals, int& number_of_non_terminals, int& number_of_productions) {
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
    return false;
  }
  if (number_of_non_terminals == 0 || number_of_terminals == 0 || number_of_productions == 0) {
    std::cerr << "Seems like your input file is incomplete, please, make sure you define a file with at leats one terminal, non-terminal and production" << std::endl;
    return false;
  }
  return true;
}

bool Grammar::ValidateProduction(const std::string& production, const Alfabeto& alphabet, const Alfabeto& non_t, char& left_side, std::string& right_side) {
  size_t space_pos = production.find(' ');
    
    // Verifica si hay un espacio y si no está al final
  if (space_pos == std::string::npos || space_pos == production.length() - 1) {
    std::cerr << "Invalid format: Production must have a space between non-terminal and the sequence." << std::endl;
    return false;
  }

  // Lado izquierdo: símbolo no terminal
  std::string left_side_string = production.substr(0, space_pos);
  if (left_side_string.length() != 1 || !non_t.ExisteSimbolo(left_side_string[0])) {
    std::cerr << "Invalid non-terminal on the left side of production: " << left_side << std::endl;
    return false;
  }
  left_side = left_side_string[0];

  // Lado derecho: secuencia de terminales y no terminales
  right_side = production.substr(space_pos + 1);
  for (char symbol : right_side) {
    if (symbol != kEmptyString && (!alphabet.ExisteSimbolo(symbol) && !non_t.ExisteSimbolo(symbol))) {
      std::cerr << "Invalid symbol in production sequence: " << symbol << std::endl;
      return false;
    }
  }
  return true;
}

bool Grammar::CheckNullProduction(const std::set<Production>& production_set, const char start_symbol) {
  // Conjunto para almacenar símbolos que producen ε
  std::set<char> nullable_symbols;

  // Primer bucle: Agregar al conjunto los símbolos que directamente producen ε
  for (const Production& prod : production_set) {
    if (prod.GetSecuence().length() == 1 && prod.GetSecuence()[0] == kEmptyString) {
      nullable_symbols.insert(prod.GetSymbol());
    }
  }
  bool changed;
  do {
    changed = false;
    for (const Production& prod : production_set) {
      const std::string& sequence = prod.GetSecuence();
      bool all_nullable = true;
      // Verificar si toda la secuencia de la producción pertenece a nullable_symbols
      for (char symbol : sequence) {
        if (nullable_symbols.find(symbol) == nullable_symbols.end()) {
          all_nullable = false;
          break;
        }
      }
      // Si toda la secuencia es anulable y el símbolo no está en el conjunto, lo agregamos
      if (all_nullable && nullable_symbols.insert(prod.GetSymbol()).second) {
        changed = true;  // Marcar que hubo un cambio
      }
    }
  } while (changed);
  if (nullable_symbols.empty()) {
    return true;
  } else if (nullable_symbols.size() == 1) {
    for (const char symbol : nullable_symbols) {
      if (symbol == start_symbol) {
        return true;
      }
    }
  } else {
    return false;
  }
}

bool Grammar::Read(std::ifstream& input_file) {
  int number_of_terminals{0};
  int number_of_non_terminals{0};
  int number_of_productions{0};
  int line_counter{1};
  char left_side;
  char start_symbol;
  std::string right_side;
  std::set<Production> productions_set;
  Alfabeto alphabet;
  Alfabeto non_t;

  if (!CheckLines(input_file, number_of_terminals, number_of_non_terminals, number_of_productions)) {
    return false;
  }

  input_file.clear(); // Limpia cualquier estado de error
  input_file.seekg(0, std::ios::beg); // Vuelve al inicio del archivo
  
  std::string line;
  while (getline(input_file, line)) {
    if (line_counter > 1 && line_counter <= number_of_terminals + 1) {
      if (line.length() > 1) {
        std::cerr << "Please, insert only symbols with 1 character on line " << line_counter << std::endl;
        return false;
      }
      char symbol = line[0];
      if (alphabet.ExisteSimbolo(symbol) || non_t.ExisteSimbolo(symbol)) {
        std::cerr << "Inavild operation: Seems like you are trying the define the same symbol twice, symbol: " << symbol << std::endl;
        return false;
      }
      alphabet.InsertarSimbolo(symbol);
    } else if (line_counter > (number_of_terminals + 2) && line_counter <= number_of_non_terminals + number_of_terminals + 2) { // To read the non-terminals
      if (line.length() > 1) {
        std::cerr << "Please, insert only symbols with 1 character on line " << line_counter << std::endl;
        return false;
      }
      char symbol = line[0];
       if (alphabet.ExisteSimbolo(symbol) || non_t.ExisteSimbolo(symbol)) {
        std::cerr << "Inavild operation: Seems like you are trying the define the same symbol twice, symbol: " << symbol << std::endl;
        return false;
      }
      if (non_t.Vacio()) {
        start_symbol = symbol;
      }
      non_t.InsertarSimbolo(symbol);
      } else if (line_counter > (number_of_non_terminals + number_of_terminals + 3) && line_counter <= (number_of_non_terminals + number_of_terminals + number_of_productions + 3)) {
        if(!ValidateProduction(line, alphabet, non_t, left_side, right_side)) {
          return false;
        } 
        Production new_production(left_side, right_side);
        if (!(productions_set.find(new_production) == productions_set.end())) {
          std::cerr << "Invalid operation, duplicated production: " << new_production << std::endl;
          return false;
        };
        productions_set.insert(new_production);
      }
    line_counter++;
  }
  if (alphabet.GetCardinal() != number_of_terminals) {
    std::cerr << "The number terminals doesn't match the specified number" << std::endl;
    return false;
  }
  if (non_t.GetCardinal() != number_of_non_terminals) {
    std::cerr << "The number non-terminals doesn't match the specified number" << std::endl;
    return false;
  }
  if (!CheckNullProduction(productions_set, start_symbol)) {
    return false;
  }

  std::cout << alphabet << std::endl;
  std::cout << non_t << std::endl;
  for (Production prod : productions_set) {
    std::cout << prod << std::endl;
  }
  std::cout << start_symbol << std::endl;
  return true;   
}

// Don't allow null prod
// Don't allow unitary prod
// Think what to do with useless symbols and productions, or symbols that are defined but never used in the productions
// There has to be at leats, one production (This is already controlled) But in that case the production has to be the start production
// Si ε ∈L(G) se permite ademñás una única producción S →ε y en este caso no se permite
// que el símbolo de arranque figure en la parte derecha de ninguna regla de producción.