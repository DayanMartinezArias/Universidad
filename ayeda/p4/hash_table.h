#pragma once

#include "secuence.h"
#include "exploration_function.h"
#include "dispersion_function.h"

template <class Key, class Container = staticSequence<Key>>
class HashTable {
 public:
  // Constructor
  HashTable(unsigned size, DispersionFunction<Key>& hf, ExplorationFunction<Key>& ef, unsigned bs)
  : tableSize(size), blockSize(bs), hashFunction(hf), explorationFunction(ef) {
    table.resize(tableSize);  // Inicializa la tabla con el tamaño especificado
  }

  // Método para insertar una clave
  bool insert(const Key& k) {
    unsigned index = hashFunction(k) % tableSize;  // Calcula la posición inicial
    unsigned attempt = 0;  // Número de intentos de exploración

    while (attempt < tableSize) {
      if (table[index].insert(k)) {  // Intenta insertar en el contenedor
        return true;  // Inserción exitosa
      }
      // Si no se pudo insertar, aplica la función de exploración
      index = (index + explorationFunction(k, attempt)) % tableSize;
      attempt++;
    }
    return false;  // Tabla llena o no se pudo insertar
  }

  // Método para buscar una clave
  bool search(const Key& k) const {
    unsigned index = hashFunction(k) % tableSize;  // Calcula la posición inicial
    unsigned attempt = 0;  // Número de intentos de exploración

    while (attempt < tableSize) {
      if (table[index].search(k)) {  // Busca en el contenedor
        return true;  // Clave encontrada
      }
      // Si no se encontró, aplica la función de exploración
      index = (index + explorationFunction(k, attempt)) % tableSize;
      attempt++;
    }
    return false;  // Clave no encontrada
  }
 private:
  unsigned tableSize;                     // Tamaño de la tabla
  unsigned blockSize;                     // Tamaño del bloque (solo para dispersión cerrada)
  DispersionFunction<Key>& hashFunction;  // Función de dispersión
  ExplorationFunction<Key>& explorationFunction;  // Función de exploración (solo para dispersión cerrada)
  std::vector<Container> table;           // Tabla de dispersión (vector de contenedores)
};

template <class Key>
class HashTable<Key, dynamicSequence<Key>> {
 public:
  // Constructor para dispersión abierta
  HashTable(unsigned size, DispersionFunction<Key>& hf)
  : tableSize(size), hashFunction(hf) {
    table.resize(tableSize);  // Inicializa la tabla con el tamaño especificado
  }

  // Método para insertar una clave
  bool insert(const Key& k) {
    unsigned index = hashFunction(k) % tableSize;  // Calcula la posición inicial
    return table[index].insert(k);  // Inserta en la secuencia dinámica
  }

    // Método para buscar una clave
  bool search(const Key& k) const {
   unsigned index = hashFunction(k) % tableSize;  // Calcula la posición inicial
    return table[index].search(k);  // Busca en la secuencia dinámica
  }
 private:
  unsigned tableSize;                     // Tamaño de la tabla
   DispersionFunction<Key>& hashFunction;  // Función de dispersión
   std::vector<dynamicSequence<Key>> table;  // Tabla de dispersión (vector de secuencias dinámicas
};