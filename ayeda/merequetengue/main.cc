#include "big_unsigned.h"
#include "big_integer.h"
#include "big_rational.h"

#include <iostream>
#include <fstream>
#include <map>
#include <sstream>


int main(int argc, char* argv[]) { 
  BigRational<10> dec, dec1;
  BigRational<8> oct, oct1;
  BigRational<16> hex, hex1;
  
  int base;
  std::string line;
  std::string num1;
  std::string num2;
  std::ifstream file(argv[1]);
  std::getline(file, line);
  std::istringstream baseStream(line);
  std::string temp;
  baseStream >> temp >> temp >> base;

  std::getline(file, num1);
  std::getline(file, num2);
  

  num1.erase(std::remove(num1.begin(), num1.end(), '/'), num1.end());
  num2.erase(std::remove(num2.begin(), num2.end(), '/'), num2.end());

  std::istringstream num1Stream(num1);
  std::istringstream num2Stream(num2);
  
  if (base == 10) {
    num1Stream >> dec;
    num2Stream >> dec1;

    std::cout << "n1 + n2 = " << (dec + dec1) << "\n";
    std::cout << "n1 - n2 = " << (dec - dec1) << "\n";
    std::cout << "n1 * n2 = " << (dec * dec1) << "\n";
    std::cout << "n1 / n2 = " << (dec / dec1) << "\n";
    std::cout << "n1 == n2 : " << (dec == dec1? "true" : "false") << "\n";
    std::cout << "n1 < n2 : " << (dec < dec1 ? "true" : "false") << "\n";
  } else if (base == 8) {
    num1Stream >> oct;
    num2Stream >> oct1;

    std::cout << "n1 + n2 = " << (oct + oct1) << "\n";
    std::cout << "n1 - n2 = " << (oct - oct1) << "\n";
    std::cout << "n1 * n2 = " << (oct * oct1) << "\n";
    std::cout << "n1 / n2 = " << (oct / oct1) << "\n";
    std::cout << "n1 == n2 : " << (oct == oct1? "true" : "false") << "\n";
    std::cout << "n1 < n2 : " << (oct < oct1? "true" : "false") << "\n";
  } else if (base == 16) {
    num1Stream >> hex;
    num2Stream >> hex1;

    std::cout << "n1 + n2 = " << (hex + hex1) << "\n";
    std::cout << "n1 - n2 = " << (hex - hex1) << "\n";
    std::cout << "n1 * n2 = " << (hex * hex1) << "\n";
    std::cout << "n1 / n2 = " << (hex / hex1) << "\n";
    std::cout << "n1 == n2 : " << (hex == hex1? "true" : "false") << "\n";
    std::cout << "n1 < n2 : " << (hex < hex1? "true" : "false") << "\n";
  }
}
