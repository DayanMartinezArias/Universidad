#include <iostream>
#include <string>
#include <fstream>
#include "point_set.h"
#include "points.h"

constexpr int Parmas{1};

struct options {
  bool help = false;
  bool visualize = false;
};

options ParseArgs(int argc, char* argv[]) {
  std::vector<std::string> args(argv + 1, argv + argc);
  options options;

  for (auto it = args.begin(), end = args.end(); it != end; ++it) {
    if (*it == "-h" || *it == "--help") {
      options.help = true;
    } else if (*it == "-d") {
      options.visualize = true;
    }
  }
  return options;
}

char Generate()  {
  static char new_symbol = 'A';
  new_symbol++; 
  if (new_symbol > 'Z') {
    return '1';
  }
  return new_symbol;
}

void help() {
  std::cout << "Usage: ./program_name [OPTIONS]\n\n"
            << "Options:\n"
            << "  -h, --help       Show this help message and exit.\n"
            << "  -d               Enable visualization of points (if supported).\n\n"
            << "Description:\n"
            << "This program computes the Euclidean Minimum Spanning Tree (EMST)\n"
            << "of a set of points. You will be prompted to input the number of points\n"
            << "and their coordinates.\n\n"
            << "Examples:\n"
            << "  ./program_name            Runs the program normally.\n"
            << "  ./program_name -h         Displays this help message.\n"
            << "  ./program_name -d         Enables visualization mode.\n";
}

int main (int argc, char* argv[]) {
  options options{ParseArgs(argc, argv)};

  if (options.help) {
    help();
    return 0;
  } 

  std::cout << "Introduce how many points you want: " << std::endl;
  CyA::point_vector points;
  std::cin >> points;
  point_set point_set(points);
  point_set.EMST();
  std::cout << "\n";
  point_set.write_tree(std::cout);
  std::cout << "\n";
  std::cout << "Total cost" << std::endl;
  std::cout << point_set.get_cost() << std::endl; 
  
  if (options.visualize) {
    if(!point_set.write_dot("EMSP.dot")) {
      return 1;
    }
  }

  return 0;
}