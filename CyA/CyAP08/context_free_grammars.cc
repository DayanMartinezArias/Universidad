#include "Production.h"
#include "grammar.h"
#include <iostream>

int main () {
  std::string sec{"aSa"};
  Production prod1('S', sec);
  std::cout << prod1 << std::endl;

  Grammar a;
  std::ifstream b("input.gra");
  a.Read(b);
  
}