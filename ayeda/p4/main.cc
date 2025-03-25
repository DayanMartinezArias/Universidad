#include <iostream>
#include <vector>
#include "dispersion_function.h"  // Asume que el código de las funciones de dispersión está en este archivo
#include "exploration_function.h"
#include "secuence.h"
#include "hash_table.h"
#include "nif.h"

void mostrarMenu() {
    std::cout << "\n--- Menú Tabla Hash ---\n";
    std::cout << "1. Insertar NIF\n";
    std::cout << "2. Buscar NIF\n";
    std::cout << "3. Salir\n";
    std::cout << "Elija una opción: ";
}

int main(int argc, char* argv[]) {
    // Configuración por defecto
    unsigned tableSize = 10;
    std::string dispersionFunc = "modulo";
    std::string hashType = "close";
    unsigned blockSize = 3;
    std::string explorationFunc = "lineal";

    // Procesar argumentos de línea de comandos
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg == "-ts" && i + 1 < argc) {
            tableSize = std::stoul(argv[++i]);
        } else if (arg == "-fd" && i + 1 < argc) {
            dispersionFunc = argv[++i];
        } else if (arg == "-hash" && i + 1 < argc) {
            hashType = argv[++i];
        } else if (arg == "-bs" && i + 1 < argc) {
            blockSize = std::stoul(argv[++i]);
        } else if (arg == "-fe" && i + 1 < argc) {
            explorationFunc = argv[++i];
        }
    }

    // Crear función de dispersión
    DispersionFunction<nif>* fd = nullptr;
    if (dispersionFunc == "modulo") {
        fd = new ModuloDispersion<nif>(tableSize);
    } else if (dispersionFunc == "suma") {
        fd = new SumaDispersion<nif>(tableSize);
    } else if (dispersionFunc == "pseudo") {
        fd = new PseudoRandomDispersion<nif>(tableSize);
    }

    // Crear tabla hash según el tipo
    if (hashType == "open") {
        HashTable<nif, dynamicSequence<nif>> tabla(tableSize, *fd);

        // Menú interactivo
        int opcion;
        do {
            mostrarMenu();
            std::cin >> opcion;
            switch (opcion) {
                case 1: {
                    long num;
                    std::cout << "Ingrese NIF (8 dígitos): ";
                    std::cin >> num;
                    if (tabla.insert(nif(num))) {
                        std::cout << "NIF insertado.\n";
                    } else {
                        std::cout << "Error al insertar.\n";
                    }
                    break;
                }
                case 2: {
                    long num;
                    std::cout << "Ingrese NIF a buscar: ";
                    std::cin >> num;
                    if (tabla.search(nif(num))) {
                        std::cout << "NIF encontrado.\n";
                    } else {
                        std::cout << "NIF no encontrado.\n";
                    }
                    break;
                }
            }
        } while (opcion != 3);
    } else {  // Dispersión cerrada
        ExplorationFunction<nif>* fe = nullptr;
        if (explorationFunc == "lineal") {
            fe = new LinearExploration<nif>();
        } else if (explorationFunc == "cuadratica") {
            fe = new QuadraticExploration<nif>();
        }

        HashTable<nif> tabla(tableSize, *fd, *fe, blockSize);

        // Menú interactivo (similar al caso abierto)
        // ...
    }

    delete fd;
    return 0;
}