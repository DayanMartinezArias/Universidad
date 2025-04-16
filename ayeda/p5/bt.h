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
    std::queue<Node<Key>*> q;
    q.push(tree.root_.get());
    int level{q.front()->GetLevel()};
    while (!q.empty()) {
      if (q.front()->GetLevel() == level) {
        os << *q.front();
      } else {
        level = q.front()->GetLevel();
        os << "Level: " << level << "\n";
        os << *q.front();
      }
      if (!q.front()->Empty()) {
        q.push(q.front()->GetLeftNode());
        q.push(q.front()->GetRightNode());
      }
      q.pop();
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

