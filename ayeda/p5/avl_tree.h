#pragma once
#include <stack>
#include <queue>
#include "bst.h"
#include "node_avl.h"

template <class Key>
class AVL : public BST<Key> {
 public:
  AVL() {};
  bool Insert(const Key& val) override;
  bool Search(const Key& val) const override;
  
 private:
  void UpdateFactors(std::stack<NodeAVL<Key>*>& st);
  void Balance(std::stack<NodeAVL<Key>*>& st);
  int GetHeight(const NodeAVL<Key>* node) const;
  void RotateLeft(NodeAVL<Key>* node);
  void RotateRight(NodeAVL<Key>* node);
};

template <class Key>
bool AVL<Key>::Insert(const Key& val) {
  std::stack<NodeAVL<Key>*> st;
  NodeAVL<Key>* current = dynamic_cast<NodeAVL<Key>*>(this->root_.get());
  NodeAVL<Key>* parent = nullptr;
  bool left_child = false;

  while (current != nullptr) {
    parent = current;
    st.push(parent);
    if (val < current->GetData()) {
      current = dynamic_cast<NodeAVL<Key>*>(current->GetLeftNode());
      left_child = true;
    } else if (val > current->GetData()) {
      current = dynamic_cast<NodeAVL<Key>*>(current->GetRightNode());
      left_child = false;
    } else {
      return false;  
    }
  }

  auto newNode = std::make_unique<NodeAVL<Key>>(val);

  if (parent == nullptr) {
    this->root_ = std::move(newNode);
  } else if (left_child) {
    parent->SetLeftNode(std::move(newNode));
  } else {
    parent->SetRightNode(std::move(newNode));
  }

  UpdateFactors(st);
  Balance(st);
  return true;
}

template <class Key>
bool AVL<Key>::Search(const Key& val) const {
  return BST<Key>::Search(val);  
}

template <class Key>
int AVL<Key>::GetHeight(const NodeAVL<Key>* node) const {
  if (node == nullptr) return 0;
  return BST<Key>::GetSubTreeHeight(node->GetData());
}

template <class Key>
void AVL<Key>::UpdateFactors(std::stack<NodeAVL<Key>*>& st) {
  while (!st.empty()) {
    NodeAVL<Key>* current = st.top();
    st.pop();

    NodeAVL<Key>* left = dynamic_cast<NodeAVL<Key>*>(current->GetLeftNode());
    NodeAVL<Key>* right = dynamic_cast<NodeAVL<Key>*>(current->GetRightNode());
    
    int left_height = GetHeight(left);
    int right_height = GetHeight(right);
    
    current->SetbalanceFactor(left_height - right_height);
  }
}

template <class Key>
void AVL<Key>::Balance(std::stack<NodeAVL<Key>*>& st) {
  // Implementation of balancing logic with rotations
}

