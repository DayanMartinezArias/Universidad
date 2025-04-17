#pragma once 
#include "bt.h"
#include "node.h"

template <class Key>
class BST : public BT<Key> {
 public:
  BST(std::unique_ptr<Node<Key>>&& root);
  bool Insert(const Key& val) override;
  bool Search(const Key& val) const override;
};

/**
 * @brief Construct a new BST object, since we only want to make sure elements are inseted using Insert() we wont allow nodes with child nodes
 * 
 * @tparam Key 
 * @param root 
 */
template <class Key>
BST<Key>::BST(std::unique_ptr<Node<Key>>&& root) : BT<Key>(std::move(root)) {
  Node<Key>* current = this->root_.get();
  if (current && !(current->GetRightNode() == nullptr && current->GetLeftNode() == nullptr)) { 
    throw std::invalid_argument("Cannot intialize tree with child nodes");
  }
}

/**
 * @brief Inserts a new Node in the binary tree
 * 
 * @tparam Key 
 * @param val 
 * @return true 
 * @return false 
 */
template <class Key>
bool BST<Key>::Insert(const Key& val) {
  Node<Key>* current = this->root_.get();
  Node<Key>* parent = nullptr;
  bool left_child = false;

  while (current != nullptr) {
    parent = current;
    if (val < current->GetData()) {
      current = current->GetLeftNode();
      left_child = true;
    } else if (val > current->GetData()) {
      current = current->GetRightNode();
      left_child = false;
    } else {
      return false;
    }
  }

  auto newNode = std::make_unique<Node<Key>>(val);

  if (parent == nullptr) {
   this->root_ = std::move(newNode);
  } else if (left_child) {
    parent->SetLeftNode(std::move(newNode));
  } else {
    parent->SetRightNode(std::move(newNode));
  }
  return true;
}

/**
 * @brief Searches for an element in the tree
 * 
 * @tparam Key 
 * @param val 
 * @return true 
 * @return false 
 */
template <class Key>
bool BST<Key>::Search(const Key& val) const {
  Node<Key>* current = this->root_.get();

  while (current != nullptr) {
    if (val < current->GetData()) {
      current = current->GetLeftNode();
    } else if (val > current->GetData()) {
      current = current->GetRightNode();
    } else if (val == current->GetData()) {
      return true;
    } 
  }
  return false;
}


