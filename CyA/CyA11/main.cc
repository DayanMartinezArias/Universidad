#include <iostream>
#include <string>
#include <fstream>
#include <expected>
#include "point_set.h"
#include "points.h"

struct options {
  bool help = false;
  bool visualize = false;
  bool inavlid_option = false;
};

options ParseArgs(int argc, char* argv[]) {
  std::vector<std::string> args(argv + 1, argv + argc);
  options options;

  for (auto it = args.begin(), end = args.end(); it != end; ++it) {
    if (*it == "-h" || *it == "--help") {
      options.help = true;
    } else if (*it == "-d") {
      options.visualize = true;
    } else {
      options.inavlid_option = true;
    }
  }
  return options;
}

void help() {
  std::cout << "Usage: ./program_name -options\n\n"
            << "  ./program_name:      Runs the program normally.\n"
            << "  ./program_name -h:   Displays this help message.\n"
            << "  ./program_name -d:   Enables visualization mode.\n";
}

int main (int argc, char* argv[]) {
  auto options{ParseArgs(argc, argv)};
  if (options.inavlid_option) {
    std::cerr << "Unknown option selected\n";
    return 1;
  }

  if (options.help) {
    help();
    return 0;
  } 

  std::cout << "Introduce how many points you want: " << std::endl;
  CyA::point_vector points;
  std::cin >> points;
  point_set point_set(points);
  std::cout << "\n";
  point_set.write_tree(std::cout);
  std::cout << "\n";
  std::cout << "Total cost" << std::endl;
  std::cout << point_set.get_cost() << std::endl; 
  
  if (options.visualize) {
    if(!point_set.write_dot("EMSP.dot")) {
      return 0;
    }
  }

  return 0;
}