#include <iostream>
#include <vector>
#include "dispersion_function.h"  // Asume que el código de las funciones de dispersión está en este archivo

int main() {
    // Tamaño de la tabla hash
    unsigned tableSize = 10;

    // Crear instancias de las funciones de dispersión
    ModuloDispersion<int> moduloFunc(tableSize);
    SumaDispersion<std::vector<int>> sumaFunc(tableSize);
    PseudoRandomDispersion<int> pseudoRandomFunc(tableSize);

    // Probar ModuloDispersion con una clave entera
    int key1 = 123;
    std::cout << "Modulo Dispersion para clave " << key1 << ": " << moduloFunc(key1) << std::endl;

    // Probar SumaDispersion con una clave de tipo vector<int>
    std::vector<int> key2 = {1, 2, 3};  // Clave como vector de enteros
    std::cout << "Suma Dispersion para clave {1, 2, 3}: " << sumaFunc(key2) << std::endl;

    // Probar PseudoRandomDispersion con una clave entera
    int key3 = 406;
    std::cout << "PseudoRandom Dispersion para clave " << key3 << ": " << pseudoRandomFunc(key3) << std::endl;

    return 0;
}