#include "node.h"
#include "bt.h"
#include <memory>
#include <iostream>

int main() {
  // Build the tree in one go using nested constructors
  std::unique_ptr<Node<int>> root = std::make_unique<Node<int>>(
    7,  // root data
    // Left subtree (8)
    std::make_unique<Node<int>>(8),
    // Right subtree (9 -> 10 -> 9)
    std::make_unique<Node<int>>(
      9,
      // Left child of 9 (10 -> 9)
      std::make_unique<Node<int>>(
        10,
        nullptr,  // No left child for 10
        std::make_unique<Node<int>>(9)  // Right child of 10 (9)
      ),
      nullptr  // No right child for 9
    )
  );

  std::cout << "Tree constructed successfully!" << std::endl;
  std::cout << *root << std::endl;

  ConcreteBT<int> tree(std::move(root));

  std::cout << "In-order traversal: ";
  tree.InOrder();
  std::cout << "\nLevel-order traversal:\n" << tree << std::endl;

  return 0;
}