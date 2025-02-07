#include <iostream>
#include <string>

int main(const int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "mamahuevo\n";
    return 1;
  }
  std::string cositas_significativas{argv[1]};
  for (char &cositas : cositas_significativas) {
    if (!std::isalpha(cositas)) {
      std::cerr << "mamahuevo2\n";
      return 1;
    }
    switch(cositas) {
      case 'Z':
        cositas = 'A';
        break;
      case 'z':
        cositas = 'a';
        break;
      default:
        cositas++;
       
    }
  }
  std::cout << cositas_significativas << std::endl;
  return 0;
}