#include "BigUnsigned.h"

int main (int argc, char* argv[]) {

  BigUnsigned num1;

  BigUnsigned num3;

  std::cin >> num1;
  std::cin >> num3;

  BigUnsigned sum = num1 - num3;
  std::cout << sum << std::endl;
}