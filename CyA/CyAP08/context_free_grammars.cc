#include "Production.h"
#include "grammar.h"
#include <iostream>

int main () {
  Grammar a;
  std::ifstream b("input1.gra");
  a.Read(b);
  std::cout << a.ChomskyNormalForm() << std::endl;
}