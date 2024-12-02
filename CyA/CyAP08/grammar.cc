#include "grammar.h"

#include <algorithm>
#include <cctype>
#include <map>
#include <vector>
#include <cctype>

/**
 * @brief Checks if all characters of a string are a digits
 * 
 * @param number 
 * @return true 
 * @return false 
 */
bool isNumber(const std::string& number) {
  return !number.empty() && std::all_of(number.begin(), number.end(), ::isdigit);
}

/**
 * @brief Returns whether the grammar is empty ot not
 * 
 * @return true 
 * @return false 
 */
bool Grammar::Empty() const {
  return productions_.empty();
}

/**
 * @brief Ensures that the number of lines of the input file is correct
 * 
 * @param input_file 
 * @param number_of_terminals 
 * @param number_of_non_terminals 
 * @param number_of_productions 
 * @return true 
 * @return false 
 */
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

/**
 * @brief Validates the correct format for all prductions in the input file
 * 
 * @param production 
 * @param alphabet 
 * @param non_t 
 * @param left_side 
 * @param right_side 
 * @return true 
 * @return false 
 */
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

/**
 * @brief Checks if theres any null production in the gtrammar
 * 
 * @param production_set 
 * @param start_symbol 
 * @return true 
 * @return false 
 */
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
  } else {
    std::cerr << "Invalid operation: Seems like you are defining a null production in your grammar, you are defining " << nullable_symbols.size() << " null productions" << std::endl;
    return false;
  }
}

/**
 * @brief Checks if there's any unitary production in the grammar
 * 
 * @param production_set 
 * @param non_t 
 * @return true 
 * @return false 
 */
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
    std::cerr << "Invalid operation: There are unitary productions implemented in your input, you are defining " << unitary_prod.size() << " unitary productions" << std::endl;
    return false;
  }
  return true;
}

/**
 * @brief Ensures that the grammar has at least one production for the start symbol
 * 
 * @param production_set 
 * @param start_symbol 
 * @return true 
 * @return false 
 */
bool Grammar::CheckDefinedStartSymbol(std::set<Production> production_set, const char start_symbol) const {
  for (const Production& prod : production_set) {
    if (prod.GetSymbol() == start_symbol) {
      return true;
    }
  }
  std::cerr << "Invalid Operation: There's no production for the start symbol" << std::endl;
  return false;
}

/**
 * @brief Reads an input file and defines the grammar
 * 
 * @param input_file 
 * @return true 
 * @return false 
 */
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
      if (!isupper(symbol) || !isalpha(symbol)) {
         std::cerr << "All non-termonal symbol must be in uppercase and must be a letter" << std::endl;
         std::cerr << "Your have defined a symbol that doesn't meet these requirements: " << symbol << std::endl;
         return false;
      }
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
 * @brief Generates a new non terminal symbol. Used when converting to CNF
 * 
 * @param new_non_t 
 * @return char 
 */
char Grammar::GenerateNewNonTerminal(const Alfabeto& new_non_t) const {
  char new_symbol = 'A';
    while (new_non_t.ExisteSimbolo(new_symbol)) {
      new_symbol++; 
      if (new_symbol > 'Z') {
        std::cerr << "Limit of Symbols for non terminals has been surpassed, cannot convert to CNF" << std::endl;
        return '1';
      }
  }
  return new_symbol;
}

/**
 * @brief Converts a grammar to CNF
 * 
 * @return Grammar 
 */
Grammar Grammar::ChomskyNormalForm() const {
  Grammar chomsky_grammar;
  if (Empty()) {
    return chomsky_grammar;
  }
  std::set<Production> prod_set; 
  std::map<char, char> associations;
  Alfabeto exits;
  Alfabeto new_non_t = non_t_;  

  // First part
  for (const Production& prod : productions_) {
    std::string right_side{prod.GetSecuence()};
    if (right_side.length() >= 2) {
      for (const char& symbol : right_side) {
        if (alphabet_.ExisteSimbolo(symbol) && !exits.ExisteSimbolo(symbol)) {
          char new_symbol = GenerateNewNonTerminal(new_non_t);
          if (new_symbol == '1') {
            return chomsky_grammar;
          }
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
  new_prods.insert(prod_set.begin(), prod_set.end());

  // Second part
  std::set<Production> final_prods;
  for (const Production& prod : new_prods) {
    std::string right_side = prod.GetSecuence();
    if (right_side.length() >= 3) {
      char current_non_terminal = prod.GetSymbol();
      for (size_t i{0}; i < right_side.length() - 2; ++i) {
        char new_non_terminal = GenerateNewNonTerminal(new_non_t);
        if (new_non_terminal == '1') {
          return chomsky_grammar;
        }
        new_non_t.InsertarSimbolo(new_non_terminal);
        Production new_prod(current_non_terminal, right_side.substr(i, 1) + new_non_terminal);
        final_prods.insert(new_prod);
        current_non_terminal = new_non_terminal;
      }
      Production new_prod(current_non_terminal, right_side.substr(right_side.length() - 2, 2));
      final_prods.insert(new_prod);
    } else {
      final_prods.insert(prod);
    }
  }
  chomsky_grammar.start_symbol_ = start_symbol_;
  chomsky_grammar.non_t_ = new_non_t;
  chomsky_grammar.productions_ = final_prods;
  chomsky_grammar.alphabet_ = alphabet_;

  return chomsky_grammar;
}

/**
 * @brief Prints all productions of the grammar
 * 
 * @param os 
 * @param obj 
 * @return std::ostream& 
 */
std::ostream& operator<<(std::ostream& os, const Grammar& obj) {
    if (obj.Empty()) {
    os << "\n";
    os << "=================================================" << "\n";
    os << "Grammar is not defined, cannot apply CNF algorith" << "\n";
    os << "Probably didn't read a file or there was an error" << "\n";
    os << "=================================================" << "\n";
    os << "\n";
    return os;
  }
  os << obj.alphabet_.GetCardinal() << "\n";
  for (const char& symbol : obj.alphabet_.GetAlphabet()) {
    os << symbol << "\n";
  }
  os << obj.non_t_.GetCardinal() << "\n";
  os << obj.start_symbol_ << "\n";
  for (const char& symbol : obj.non_t_.GetAlphabet()) {
    if (symbol == obj.start_symbol_) {
      continue;
    }
    os << symbol << "\n";
  }
  os << obj.productions_.size() << "\n";
  for (const Production& prod : obj.productions_) {
    os << prod.GetSymbol() << " " << prod.GetSecuence() << "\n";
  }
  return os;
}

/**
 * @brief Prints more visual info about the grammar
 * 
 */
void Grammar::PrintInfo() const {
 if (Empty()) {
    std::cout << "\n";
    std::cout << "=================================================" << "\n";
    std::cout << "Grammar is not defined, cannot apply CNF algorith" << "\n";
    std::cout << "Probably didn't read a file or there was an error" << "\n";
    std::cout << "=================================================" << "\n";
    std::cout << "\n";
  }
  std::cout << "CONTEXT FREE GRAMMAR" << "\n\n";
  std::cout << "Terminals: " << "\n";
  std::cout << alphabet_ << "\n";

  std::cout << "Non-terminals: " << "\n";
  std::cout << non_t_ << "\n";

  std::cout << "Productions: " << "\n";
  if (productions_.empty()) {
    std::cout << "Empty set, not valid" << "\n";
  }
  std::cout << "{";
  bool primero = true;  
  for (const Production symbol : productions_) { 
    if (!primero) {
      std::cout << ", "; 
    }
   std::cout << symbol;
   primero = false;  
  }
  std::cout << "}";
}
