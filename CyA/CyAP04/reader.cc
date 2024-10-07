#include "reader.h"
#include <fstream>
#include <iostream>

FileReader::FileReader(const std::string& file_name) : file_name_(file_name) {}

std::vector<std::string> FileReader::ReadFile() {
  std::ifstream input(file_name_);
  if (!input.is_open()) {
    std::cerr << "Could not open file: " << file_name_ << "\n";
    return {};
  }

  std::string line;
  std::vector<std::string> content;
  while (getline(input, line)) {
    content.push_back(line);
  }
  return content;
}
