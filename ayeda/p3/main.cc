#include <iostream>
#include <fstream>
#include <string>
#include <utility>
#include <stack>
#include <map>
#include <memory>
#include <sstream>

#include "big_number.h"
#include "big_rational.h"

template <unsigned char Base>
struct Board {
  std::map<std::string, BigNumber<Base>*> board;
};

template <unsigned char Base>
void ProcessFile(std::ofstream& output, std::ifstream& input, int line_number) {
  Board<Base> board;

  if (!input || !output) {
    std::cerr << "There was an error opening file" << std::endl;
    return;
  }
  std::string line;

  while (std::getline(input, line)) {
    if(line_number == 1) {
      ++line_number;
      continue;
    }
    else {
      std::istringstream ss(line);
      std::string label, op;
      ss >> label >> op;
      if (op == "=") {
        std::string value;
        ss >> value;
        try {
          auto num1 = BigNumber<Base>::create(value.c_str());
          board.board[label] = std::move(num1);
        } catch (BigNumberException e) {
          std::cerr << e.what() << std::endl;
          return;
        }
      } else if (op == "?") {
        std::stack<BigNumber<Base>*> pila;
        std::string token;
        try {
          while (ss >> token) {
            if (board.board.count(token)) {
              pila.push(board.board[token]);
            } else {
              if (pila.size() < 2) throw std::runtime_error("Invalid Expression");

              auto b = pila.top();
              pila.pop();
              auto a = pila.top();
              pila.pop();

              if (token == "+") pila.push(a->add(*b));
              else if (token == "-") pila.push(a->sub(*b));
              else if (token == "*") pila.push(a->multiplicate(*b));
              else if (token == "/") pila.push(a->divide(*b));
              else {
                throw std::runtime_error("Invalid Expression");
              }
            }
          }
          if (pila.size() != 1) {
            throw std::runtime_error("Invalid Expression");
          }
          board.board[label] = pila.top();
        } catch (BigNumberException e) {
          std::cerr << e.what() << std::endl;
        }
      }
      if (board.board[label] == nullptr) {
        std::cerr << "Error: Variable '" << label << "' initialized." << std::endl;
      } else {
        output << label << " = " << *board.board[label] << std::endl;
      }
    }
  }
}


int main (int argc, char* argv[]) {
  unsigned base;
  std::string filename("input.txt");
  std::string out("output.txt");
  std::ifstream input(filename);
  std::ofstream output(out);
  int line_number{1};
  std::string line;

   while (std::getline(input, line)) {
    if(line_number == 1) {
      std::istringstream ss(line);
      std::string temp;
      ss >> temp >> temp >> base;
      if (temp != "=" || base <= 0) {
        std::cerr << "Error: Invalid format" << std::endl;
          return 1;
        }
      output << "Base = " << base << std::endl;
      ++line_number;
    }
  }
  input.clear();
  input.seekg(0, std::ios::beg);
  if (base == 10) {
    ProcessFile<10>(output, input, line_number);
  } else if (base == 2) {
     ProcessFile<2>(output, input, line_number);
  } else if (base == 8) {
     ProcessFile<8>(output, input, line_number);
  } else if (base == 16) {
     ProcessFile<16>(output, input, line_number);
  }
  return 0;
}