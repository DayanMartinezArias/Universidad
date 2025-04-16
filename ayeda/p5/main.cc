#include "node.h"
#include "bt.h"
#include <memory>
#include <iostream>

int main () {
  std::unique_ptr<Node<int>> n1 = std::make_unique<Node<int>>(7);
  std::unique_ptr<Node<int>> n2 = std::make_unique<Node<int>>(8);
  std::unique_ptr<Node<int>> n3 = std::make_unique<Node<int>>(9);

  n1->SetLeftNode(std::move(n2));
  n1->SetRightNode(std::move(n3));

  std::cout << *n1 << std::endl;

  ConcreteBT<int> tree(std::move(n1));

  tree.InOrder();  // Para probar el recorrido
  std::cout << tree << std::endl;

  return 0;
}
