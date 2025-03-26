#include <iostream>
#include <vector>
#include <expected>
#include <string>
#include "dispersion_function.h"  
#include "exploration_function.h"
#include "secuence.h"
#include "hash_table.h"
#include "nif.h"

void Menu() {
    std::cout << "\n--- Menú Tabla Hash ---\n";
    std::cout << "1. Insertar NIF\n";
    std::cout << "2. Buscar NIF\n";
    std::cout << "3. Salir\n";
    std::cout << "Elija una opción: ";
}

struct options {
  unsigned sz = 10;
  std::string disp = "modulo";
  bool open_hash = false;
  unsigned bl_sz = 10;
  std::string exp = "lineal";
};

options parse_args(int argc, char* argv[]) {
  std::vector<std::string> args(argv + 1, argv + argc);
  options program_options;

  for (auto it = args.begin(); it != args.end(); ++it) {
    if (*it == "-ts") {
      if (++it == args.end()) {
        throw std::runtime_error("Missing value for -ts");
      }
      program_options.sz = std::stoul(*it);
    }
    else if (*it == "-fd") {
      if (++it == args.end()) {
        throw std::runtime_error("Missing value for -fd");
      }
      program_options.disp = *it;
    }
    else if (*it == "-hash") {
      if (++it == args.end()) {
        throw std::runtime_error("Missing value for -hash");
      }
      if (*it == "open") {
        program_options.open_hash = true;
      }
      else if (*it == "close") {
        program_options.open_hash = false;
      }
      else {
        throw std::runtime_error("Invalid value for -hash, must be 'open' or 'close'");
      }
    }
    else if (*it == "-bs") {
      if (!program_options.open_hash) {
        if (++it == args.end()) {
          throw std::runtime_error("Missing value for -bs");
        }
        program_options.bl_sz = std::stoul(*it);
      }
      else {
        throw std::runtime_error("-bs only valid for closed hashing");
      }
    }
    else if (*it == "-fe") {
      if (!program_options.open_hash) {
        if (++it == args.end()) {
          throw std::runtime_error("Missing value for -fe");
        }
        program_options.exp = *it;
      }
      else {
        throw std::runtime_error("-fe only valid for closed hashing");
      }
    }
    else {
      throw std::runtime_error("Unknown option: " + *it);
    }
  }

  if (program_options.sz == 0) {
    throw std::runtime_error("Table size (-ts) must be specified");
  }
  if (program_options.disp.empty()) {
    throw std::runtime_error("Hash function (-fd) must be specified");
  }
  if (!program_options.open_hash && program_options.bl_sz == 0) {
    throw std::runtime_error("Block size (-bs) must be specified for closed hashing");
  }

  return program_options;
}


int main(int argc, char* argv[]) {
  try {
    options opts = parse_args(argc, argv);

    // Creamos la función de dispersión
    DispersionFunction<nif>* fd = nullptr;
    if (opts.disp == "modulo") {
      fd = new ModuloDispersion<nif>(opts.sz);
    } else if (opts.disp == "suma") {
      fd = new SumaDispersion<nif>(opts.sz);
    } else if (opts.disp == "pseudo") {
      fd = new PseudoRandomDispersion<nif>(opts.sz);
    } 

    if (opts.open_hash == true) {
      HashTable<nif, dynamicSequence<nif>> table(opts.sz, *fd);
      int option;
      do {
        Menu();
        std::cin >> option;
        switch (option) {
          case 1:
            long unsigned num1;
            std::cout << "Inserte NIF:";
            std::cin >> num1;
            if (table.insert(nif(num1))) {
              std::cout << "NIF encontrado" << std::endl;
            } else {
              std::cout << "No se pudo insertar" << std::endl;
            }
            break;
          case 2:
            long unsigned num2;
            std::cout << "Ingrese NIF a buscar";
            std::cin >> num2;
            if (table.search(nif(num2))) {
              std::cout << "NIF encontrado" << std::endl;
            } else {
              std::cout << "NIF no encontrado" << std::endl;
            }
            break;     
        }
      } while(option != 3);
      delete fd;
    } else {
      ExplorationFunction<nif>* fe = nullptr;
      if (opts.exp == "lineal") fe = new LinearExploration<nif>();
      else if (opts.exp == "cuadratica") fe = new QuadraticExploration<nif>();
      else if (opts.exp == "doble") fe = new DoubleHashingExploration<nif>(*fd);
      else if (opts.exp == "random") fe = new RandomExploration<nif>();

      HashTable<nif, staticSequence<nif>> table(opts.sz, *fd, *fe, opts.bl_sz);  

      int option;
      do {
        Menu();
        std::cin >> option;
        switch (option) {
          case 1:
            long unsigned num1;
            std::cout << "Inserte NIF:";
            std::cin >> num1;
            if (table.insert(nif(num1))) {
              std::cout << "Insertado" << std::endl;
            } else {
              std::cout << "No se pudo insertar" << std::endl;
            }
            break;
          case 2:
            long unsigned num2;
            std::cout << "Ingrese NIF a buscar:";
            std::cin >> num2;
            if (table.search(nif(num2))) {
              std::cout << "NIF encontado" << std::endl;
            } else {
              std::cout << "NIF no encontrado" << std::endl;
            }
            break;     
        }
      } while(option != 3);
      delete fd;
      delete fe;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  return 0;
}
