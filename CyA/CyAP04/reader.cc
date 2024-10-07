

#include "reader.h"
#include <fstream>
#include <iostream>

/**
 * @brief Construct a new File Reader:: File Reader object
 * 
 * @param file_name 
 */
FileReader::FileReader(const std::string& file_name) : file_name_(file_name) {}

/**
 * @brief Reads the file content
 * 
 * @return std::vector<std::string> 
 */
std::vector<std::string> FileReader::ReadFile() {
  std::ifstream input(file_name_);
  if (!input.is_open()) {
    std::cerr << "No se pudo abrir el fichero " << file_name_ << "\n";
    return {};
  }

  std::string line;
  std::vector<std::string> content;
  while (getline(input, line)) {
    content.push_back(line);
  }
  return content;
}
