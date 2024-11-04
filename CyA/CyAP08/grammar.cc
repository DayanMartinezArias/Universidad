#include "grammar.h"

#include <algorithm>
#include <cctype>
#include <map>

bool isNumber(const std::string& number) {
  return !number.empty() && std::all_of(number.begin(), number.end(), ::isdigit);
}

bool Grammar::CheckLines(std::ifstream& input_file, int& number_of_terminals, int& number_of_non_terminals, int& number_of_productions) const{
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

bool Grammar::ValidateProduction(const std::string& production, const Alfabeto& alphabet, const Alfabeto& non_t, char& left_side, std::string& right_side) const {
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
  
  std::string new_right_side;
  if (right_side.length() != 1) {
    for (char symbol : right_side) {
      if (symbol != kEmptyString) {
        new_right_side += symbol;
      }
    }
    right_side = new_right_side;
  }
  return true;
}

bool Grammar::CheckNullProduction(const std::set<Production>& production_set, const char start_symbol) const {
  // Conjunto para almacenar símbolos que producen ε
  std::set<char> nullable_symbols;

  // Primer bucle: Agregar al conjunto los símbolos que directamente producen ε
  for (const Production& prod : production_set) {
    if (prod.GetSecuence().length() == 1 && prod.GetSecuence()[0] == kEmptyString) {
      nullable_symbols.insert(prod.GetSymbol());
    }
  }
  if (nullable_symbols.empty()) {
    return true;
  } else if (nullable_symbols.size() == 1 && nullable_symbols.count(start_symbol)) {
    return true;
  } else {
    std::cerr << "Invalid operation: Seems like you are defining a null production in your grammar" << std::endl;
    return false;
  }
}

bool Grammar::CheckUnitaryproductions(const std::set<Production>& production_set, const Alfabeto& non_t) const {
  std::set<Production> unitary_prod;
  for (const Production& prod: production_set) {
    if (prod.GetSecuence().length() == 1) {
      if (non_t.ExisteSimbolo(prod.GetSecuence()[0])) {
        unitary_prod.insert(prod);
      }
    }
  }
  if (!unitary_prod.empty()) {
    std::cerr << "Invalid operation: There are unitary productions implemented in your input" << std::endl;
    return false;
  }
  return true;
}

bool Grammar::CheckDefinedStartSymbol(std::set<Production> production_set, const char start_symbol) const {
  for (const Production& prod : production_set) {
    if (prod.GetSymbol() == start_symbol) {
      return true;
    }
  }
  std::cerr << "Invalid Operation: There's no production for the start symbol" << std::endl;
  return false;
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

  input_file.clear();
  input_file.seekg(0, std::ios::beg);
  
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
  if (!CheckUnitaryproductions(productions_set, non_t)) {
    return false;
  }
  if (!CheckDefinedStartSymbol(productions_set, start_symbol)) {
    return false;
  }

  start_symbol_ = start_symbol;
  productions_ = productions_set;
  non_t_ = non_t;
  alphabet_ = alphabet;

  return true;   
}

/**
1: for all (A → X1X2 . . . Xn (con n ≥ 2, Xi ∈ (Σ ∪ V )) do
2: for all (Xi) do
3: if (Xi = a ∈ Σ) then
4: Add the production Ca → a;
5: Replace Xi with Ca in A → X1X2 . . . Xn;
6: end if
7: end f
*/

char Grammar::GenerateNewNonTerminal(const Alfabeto& new_non_t) const {
  char new_symbol = 'A';
    while (new_non_t.ExisteSimbolo(new_symbol)) {
      new_symbol++; 
  }
  return new_symbol;
}

Grammar Grammar::ChomskyNormalForm() const {
  Grammar chomsky_grammar;
  std::set<Production> prod_set; 
  std::map<char, char> associations;
  Alfabeto exits;
  Alfabeto new_non_t = non_t_;  // new alphabet
  for (const Production& prod : productions_) {
    std::string right_side{prod.GetSecuence()};
    if (right_side.length() >= 2) {
      for (const char& symbol : right_side) {
        if (alphabet_.ExisteSimbolo(symbol) && !exits.ExisteSimbolo(symbol)) {
          char new_symbol = GenerateNewNonTerminal(new_non_t);
          new_non_t.InsertarSimbolo(new_symbol);
          exits.InsertarSimbolo(symbol);
          std::string new_sec(1, symbol);
          Production new_prod(new_symbol, new_sec);
          prod_set.insert(new_prod);
          associations[symbol] = new_symbol; 
        }
      }
    }
  }
  std::set<Production> new_prods;
  std::string new_right;
  for (const Production& prod : productions_) {
    std::string right_side{prod.GetSecuence()};
    if (right_side.length() >= 2) {
      for (const char sym : right_side) {
        if (associations.find(sym) != associations.end())  {
          new_right += associations[sym];
        } else {
          new_right += sym;
        }
      }
    Production f_prod(prod.GetSymbol(), new_right);
    new_prods.insert(f_prod);
    new_right.clear();
    } else {
      Production f_prod(prod.GetSymbol(), right_side);
      new_prods.insert(f_prod);
    } 
  }

  for (const auto& as : associations) {
    std::cout << as.first << "|" << as.second << std::endl;
  }
   std::cout << "new_prodssssss" << std::endl;
  for (const Production& prod : new_prods) {
    std::cout << prod << std::endl;
  }
  return chomsky_grammar;
}

std::ostream& operator<<(std::ostream& os, const Grammar& obj) {
  os << "CONTEXT FREE GRAMMAR" << "\n\n";
  os << "Terminals: " << "\n";
  os << obj.alphabet_ << "\n";

  os << "Non-terminals: " << "\n";
  os << obj.non_t_ << "\n";

  os << "Productions: " << "\n";
  os << "{";
  bool primero = true;  
  for (const Production symbol : obj.productions_) { 
    if (!primero) {
      os << ", "; 
    }
   os << symbol;
   primero = false;  
  }
  os << "}";

  return os;
}

