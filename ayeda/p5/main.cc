#include "bst.h"
#include "node_avl.h"
#include "avl_tree.h"
#include <iostream>

int main() {
  AVL<int> tree;

  // Insertar elementos
  std::cout << "Insertando: 30, 10, 20, 40, 50\n";
  tree.Insert(30);
  tree.Insert(10);
  tree.Insert(20);
  tree.Insert(40);
  tree.Insert(50);

  // Buscar elementos
  std::cout << "Buscando 20: " << (tree.Search(20) ? "Encontrado" : "No encontrado") << "\n";
  std::cout << "Buscando 99: " << (tree.Search(99) ? "Encontrado" : "No encontrado") << "\n";

  // Mostrar en preorden
  std::cout << "Preorden del árbol:\n";

  return 0;
}