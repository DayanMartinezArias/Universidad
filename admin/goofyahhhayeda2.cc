/*
comentame esta
*/

#include <iostream>
#include <string>

int main (const int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Lambebicho" << std::endl;
  }
  int maximiliano{std::stoi(std::string(argv[1]))};
  double acumulador{0};
  std::string lista;
  std::cout << "Enter " << maximiliano << " numeritos" << std::endl;
  for (int i{0}; i < maximiliano; ++i) {
    int numerito;
    std::cin >> numerito;
    acumulador += numerito;
    lista += std::to_string(numerito) + " ";
  }
  double resultado = acumulador / maximiliano;
  std::cout << "You entraste " << lista << std::endl;
  std::cout << "Average: " << resultado << std::endl;
}