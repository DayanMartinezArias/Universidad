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
  void RotateLeft(NodeAVL<Key>* node, NodeAVL<Key>* parent);
  void RotateRight(NodeAVL<Key>* node, NodeAVL<Key>* parent);
  NodeAVL<Key>* FindParent(NodeAVL<Key>* node);
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
NodeAVL<Key>* AVL<Key>::FindParent(NodeAVL<Key>* node) {
  if (node == nullptr || node == this->root_.get()) return nullptr;
  
  NodeAVL<Key>* current = dynamic_cast<NodeAVL<Key>*>(this->root_.get());
  NodeAVL<Key>* parent = nullptr;
  
  while (current != nullptr && current != node) {
    parent = current;
    if (node->GetData() < current->GetData()) {
      current = dynamic_cast<NodeAVL<Key>*>(current->GetLeftNode());
    } else {
      current = dynamic_cast<NodeAVL<Key>*>(current->GetRightNode());
    }
  }
  
  return parent;
}

template <class Key>
void AVL<Key>::Balance(std::stack<NodeAVL<Key>*>& st) {
  while (!st.empty()) {
    NodeAVL<Key>* current = st.top();
    st.pop();

    int bf = current->GetbalanceFactor();
    NodeAVL<Key>* parent = FindParent(current);

    if (bf > 1) {
      // Subárbol izquierdo más alto
      NodeAVL<Key>* left = dynamic_cast<NodeAVL<Key>*>(current->GetLeftNode());
      if (left->GetbalanceFactor() >= 0) {
        RotateRight(current, parent);  // Rotación simple derecha
      } else {
        RotateLeft(left, current);      // Rotación izquierda en hijo
        RotateRight(current, parent);  // Luego rotación derecha
      }
    } else if (bf < -1) {
      // Subárbol derecho más alto
      NodeAVL<Key>* right = dynamic_cast<NodeAVL<Key>*>(current->GetRightNode());
      if (right->GetbalanceFactor() <= 0) {
        RotateLeft(current, parent);   // Rotación simple izquierda
      } else {
        RotateRight(right, current);   // Rotación derecha en hijo
        RotateLeft(current, parent);    // Luego rotación izquierda
      }
    }
  }
}

template <class Key>
void AVL<Key>::RotateLeft(NodeAVL<Key>* x, NodeAVL<Key>* parent) {
  NodeAVL<Key>* y = dynamic_cast<NodeAVL<Key>*>(x->GetRightNode());
  auto B = y->GetLeftNode() ? std::unique_ptr<Node<Key>>(y->GetLeftNode()) : nullptr;

  // Reasignar hijos
  x->SetRightNode(std::move(B));
  y->SetLeftNode(std::unique_ptr<NodeAVL<Key>>(x));

  // Actualizar la referencia del padre
  if (parent == nullptr) {
    this->root_.release();
    this->root_ = std::unique_ptr<NodeAVL<Key>>(y);
  } else {
    if (parent->GetLeftNode() == x) {
      parent->SetLeftNode(std::unique_ptr<NodeAVL<Key>>(y));
    } else {
      parent->SetRightNode(std::unique_ptr<NodeAVL<Key>>(y));
    }
  }
}

template <class Key>
void AVL<Key>::RotateRight(NodeAVL<Key>* y, NodeAVL<Key>* parent) {
  NodeAVL<Key>* x = dynamic_cast<NodeAVL<Key>*>(y->GetLeftNode());
  auto B = x->GetRightNode() ? std::unique_ptr<Node<Key>>(x->GetRightNode()) : nullptr;

  // Reasignar hijos
  y->SetLeftNode(std::move(B));
  x->SetRightNode(std::unique_ptr<NodeAVL<Key>>(y));

  // Actualizar la referencia del padre
  if (parent == nullptr) {
    this->root_.release();
    this->root_ = std::unique_ptr<NodeAVL<Key>>(x);
  } else {
    if (parent->GetLeftNode() == y) {
      parent->SetLeftNode(std::unique_ptr<NodeAVL<Key>>(x));
    } else {
      parent->SetRightNode(std::unique_ptr<NodeAVL<Key>>(x));
    }
  }
}