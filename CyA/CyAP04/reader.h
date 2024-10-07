#ifndef READER_H
#define READER_H

#include <string>
#include <vector>

class FileReader {
public:
  FileReader(const std::string& file_name);
  std::vector<std::string> ReadFile();

private:
  std::string file_name_;
};

#endif