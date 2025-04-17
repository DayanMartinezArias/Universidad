#pragma once
#include "bst.h"
#include "node_avl.h"

template <class Key>
class AVL : public BST<Key> {
 public:
  AVL() : avl_root_(nullptr) {};
  bool Insert(const Key& val);
 // bool Search(const Key& val) const;
 private:
  std::unique_ptr<NodeAVL<Key>> avl_root_;
};

template <class Key>
bool AVL<Key>::Insert(const Key& val) {
  NodeAVL<Key>* current = avl_root_.get();
  NodeAVL<Key>* parent = nullptr;
  bool left_child = false;

  while (current != nullptr) {
    parent = current;
    if (val < current->GetData()) {
      current = dynamic_cast<NodeAVL<Key>*>(current->GetLeftNode());
      left_child = true;
    } else if (val > current->GetData()) {
      current = dynamic_cast<NodeAVL<Key>*>(current->GetRightNode());
      left_child = false;
    } else {
      return false;  // valor duplicado
    }
  }

  auto newNode = std::make_unique<NodeAVL<Key>>(val);

  if (parent == nullptr) {
    avl_root_ = std::move(newNode);
  } else if (left_child) {
    parent->SetLeftNode(std::move(newNode));
  } else {
    parent->SetRightNode(std::move(newNode));
  }

  return true;
}
