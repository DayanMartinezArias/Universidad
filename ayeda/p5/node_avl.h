#pragma once
#include "node.h"

template <class Key>
class NodeAVL : public Node<Key> {
 public:
  NodeAVL();
  NodeAVL(const Key& data, const int& level = 0);
  int GetbalanceFactor() const {return balance_factor;}
  void SetbalanceFactor(const int& new_balance_factor) {balance_factor = new_balance_factor;}
 private:
  int balance_factor;
};

// Default constructor
template <class Key>
NodeAVL<Key>::NodeAVL() : Node<Key>(), balance_factor(0) {}

// Constructor with data
template <class Key>
NodeAVL<Key>::NodeAVL(const Key& data, const int& level) : Node<Key>(data, level), balance_factor(0) {}
