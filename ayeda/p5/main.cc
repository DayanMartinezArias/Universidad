#include "bst.h"
#include <iostream>

int main() {
    try {
        // Create BST with root node 50
        BST<int> bst(std::make_unique<Node<int>>(50));

        // Insert values
        std::cout << "Insert 30: " << (bst.Insert(30) ? "Success" : "Failed") << std::endl;
        std::cout << "Insert 70: " << (bst.Insert(70) ? "Success" : "Failed") << std::endl;
        std::cout << "Insert 20: " << (bst.Insert(20) ? "Success" : "Failed") << std::endl;
        std::cout << "Insert 40: " << (bst.Insert(40) ? "Success" : "Failed") << std::endl;

        // Search for values
        std::cout << "\nSearch results:" << std::endl;
        std::cout << "Search 30: " << (bst.Search(30) ? "Found" : "Not found") << std::endl;
        std::cout << "Search 25: " << (bst.Search(25) ? "Found" : "Not found") << std::endl;
        std::cout << "Search 70: " << (bst.Search(70) ? "Found" : "Not found") << std::endl;

        bst.InOrder();
        std::cout << bst << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}