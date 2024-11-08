/**
 * Dayán Martínez Arias
 * alu0101644561
 * SSOO
 *
 */

#include <cstdlib>
#include <iostream>
#include <string>

std::string getenv(const std::string& name)
{
char* value = getenv(name.c_str());
if (value) {
return std::string(value);
}
else {
return std::string();
}
}

/**
 * @brief 
 * 
 * @return int 
 */
int main () {
  std::cout << getenv("PATH") << std::endl;
}