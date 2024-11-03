#include "Production.h"
#include "grammar.h"
#include <iostream>

int main () {
  Grammar a;
  std::ifstream b("input.gra");
  a.Read(b);
  std::cout << a << std::endl;
  std::cout << a.ChomskyNormalForm() << std::endl;

}