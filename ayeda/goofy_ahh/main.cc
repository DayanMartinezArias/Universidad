#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <chrono>
#include "static_sequence.h"
#include "sort_method.h"
#include "nif.h"

struct options {
  unsigned size;
  std::string method;
  std::string init;
  std::string filename;
  bool trace;
};

options parse_args(int argc, char* argv[]) {
  options program_options;
  program_options.trace = false; 

  for (int i = 1; i < argc; ++i) {
    std::string arg = argv[i];

    if (arg == "-size") {
      if (i + 1 >= argc) throw std::runtime_error("Missing value for -size");
      program_options.size = std::stoul(argv[++i]);
    } else if (arg == "-ord") {
      if (i + 1 >= argc) throw std::runtime_error("Missing value for -ord");
      program_options.method = argv[++i];
    } else if (arg == "-init") {
      if (i + 1 >= argc) throw std::runtime_error("Missing value for -init");
      program_options.init = argv[++i];
      if (program_options.init == "file") {
        if (i + 1 >= argc) throw std::runtime_error("Missing filename for -init file");
          program_options.filename = argv[++i];
        }
    } else if (arg == "-trace") {
      if (i + 1 >= argc) throw std::runtime_error("Missing value for -trace");
      std::string trace_val = argv[++i];
      program_options.trace = (trace_val == "yes");
    }
  }
  return program_options;
}

StaticSequence<Nif> initialize_sequence(const options& opts) {
  StaticSequence<Nif> seq(opts.size);

  if (opts.init == "manual") {
    std::cout << "Enter " << opts.size << " values to insert in the secuence:" << std::endl;
    for (unsigned i = 0; i < opts.size; ++i) {
      unsigned num;
      std::cin >> num;
      seq[i] = Nif(num);
    }
  } else if (opts.init == "random") {
    std::srand(std::time(0));
    for (unsigned i = 0; i < opts.size; ++i) {
      unsigned random_number = 10000000 + std::rand() % 90000000;
      seq[i] = Nif(random_number);
    }
  } else if (opts.init == "file") {
    std::ifstream file(opts.filename);
    if (!file.is_open()) throw std::runtime_error("Could not open the file");

    for (unsigned i = 0; i < opts.size; ++i) {
      unsigned num;
      file >> num;
      if (file.eof()) throw std::runtime_error("There are not enough numbers in file to fill the secuence");
      seq[i] = Nif(num);
    }
  } else {
    throw std::runtime_error("Initialization method does not exists");
  }
  return seq;
}

SortMethod<Nif>* create_sort_method(const std::string& method, StaticSequence<Nif>& seq, bool trace) {
  if (method == "insertion") return new InsertionSortMethod<Nif>(seq, trace);
  if (method == "shake") return new ShakeSortMethod<Nif>(seq, trace);
  if (method == "quicksort") return new QuickSortMethod<Nif>(seq, trace);
  if (method == "heapsort") return new HeapSortMethod<Nif>(seq, trace);
  if (method == "shellsort") return new ShellSortMethod<Nif>(seq, 0.5, trace);
  throw std::runtime_error("Invalid sorting method");
}

int main(int argc, char* argv[]) {
  try {
    options opts = parse_args(argc, argv);
    StaticSequence<Nif> seq = initialize_sequence(opts);
    std::cout << "Original:" << std::endl;
    seq.PrintTable();
    SortMethod<Nif>* sorter = create_sort_method(opts.method, seq, opts.trace);
    sorter->Sort();
    std::cout << "Sorted:" << std::endl;
    seq.PrintTable();
    delete sorter;
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}