#pragma once
#include <queue>
#include "node.h"

template <class Key>
class BT {
 public:
  BT() { root_ = nullptr; }
  BT(std::unique_ptr<Node<Key>>&& root) : root_(std::move(root)) {}

  virtual bool Insert(const Key& val) = 0;
  virtual bool search(const Key& val) const = 0;
  virtual void InOrder() const;

friend std::ostream& operator<<(std::ostream& os, const BT<Key>& tree) {
  if (!tree.root_) return os;  // Handle empty tree

  std::queue<Node<Key>*> q;
  q.push(tree.root_.get());
  int current_level = tree.root_->GetLevel();
  os << "Level: " << current_level << "\n";
    
  while (!q.empty()) {
    if (q.front() == nullptr) {
      os << "[.]";
      q.pop();
      continue;
    }
    Node<Key>* current = q.front();
    q.pop();
        
    // Print level header if we've moved to a new level
    if (current->GetLevel() != current_level) {
      current_level = current->GetLevel();
      os << "\nLevel: " << current_level << "\n";
    }
        
    os << *current << " ";  // Print the node     
    // Push children
    if (current->GetLeftNode()) {
      q.push(current->GetLeftNode());
    } else {
      q.push(nullptr);
    }
    if (current->GetRightNode()) {
      q.push(current->GetRightNode());
    } else {
      q.push(nullptr);
    }
    }
    return os;
}

 protected:
  void InOrderHelper(const Node<Key>* node) const;

 private:
  std::unique_ptr<Node<Key>> root_;
};

template <class Key>
void BT<Key>::InOrder() const {
  InOrderHelper(root_.get());
  std::cout << std::endl;
}

template <class Key>
void BT<Key>::InOrderHelper(const Node<Key>* node) const {
  if (node == nullptr) return;
  InOrderHelper(node->GetLeftNode());
  std::cout << node->GetData() << " ";
  InOrderHelper(node->GetRightNode());
}

// Subclase concreta
template <class Key>
class ConcreteBT : public BT<Key> {
 public:
  using BT<Key>::BT;

  bool Insert(const Key& val) override {
    // Implementación ficticia
    return true;
  }

  bool search(const Key& val) const override {
    // Implementación ficticia
    return false;
  }
};

