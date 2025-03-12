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
void ProcessFile() {
  unsigned base;
  std::string filename("input.txt");
  std::string out("output.txt");
  std::ifstream input(filename);
  std::ofstream output(out);

  Board<Base> board;

  if (!input) {
    std::cerr << "There was an error opening file" << std::endl;
    return;
  }
  std::string line;
  int line_number{1};

  while (std::getline(input, line)) {
    if(line_number == 1) {
      std::istringstream ss(line);
      std::string temp;
      ss >> temp >> temp >> base;
      if (temp != "=" || base <= 0) {
        std::cerr << "Error: Invalid format" << std::endl;
          return;
        }
      output << "Base = " << base << std::endl;
      ++line_number;
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
            }
          }
         // if (pila.size() != 1) throw runtime_error("Invalid expresion");
          board.board[label] = pila.top();
        } catch (BigNumberException e) {

        }
      }
      output << label << " = " << *board.board[label] << std::endl;
    }
  }
}


int main (int argc, char* argv[]) {
  ProcessFile<10>();
  return 0;
}