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
  if (!tree.root_) return os;

  std::queue<std::pair<Node<Key>*, int>> q;
  q.push({tree.root_.get(), tree.root_->GetLevel()});
  int current_level = tree.root_->GetLevel();
  os << "Level: " << current_level << "\n";

  while (!q.empty()) {
    auto [current, level] = q.front();
    q.pop();

    if (level != current_level) {
      current_level = level;
      os << "\nLevel: " << current_level << "\n";
    }

    if (current == nullptr) {
      os << "[.] ";
      continue;
    }

    os << *current << " ";

    // Push children with incremented level
    q.push({current->GetLeftNode(), level + 1});
    q.push({current->GetRightNode(), level + 1});
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

