#include <iostream>
#include <vector>
#include "dispersion_function.h"  // Asume que el código de las funciones de dispersión está en este archivo
#include "exploration_function.h"
#include "secuence.h"
#include "hash_table.h"

int main() {
    // Tamaño de la tabla
    unsigned tableSize = 10;

    // Función de dispersión (módulo)
    ModuloDispersion<int> hashFunction(tableSize);

    // Crear la tabla hash con dispersión abierta
    HashTable<int, dynamicSequence<int>> hashTable(tableSize, hashFunction);

    // Insertar algunas claves
    hashTable.insert(123);
    hashTable.insert(456);
    hashTable.insert(789);

    // Buscar una clave
    bool found = hashTable.search(456);
    if (found) {
        std::cout << "Clave 123 encontrada." << std::endl;
    } else {
        std::cout << "Clave 123 no encontrada." << std::endl;
    }

    return 0;
}