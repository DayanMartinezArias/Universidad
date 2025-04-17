#pragma once
#include <memory>
#include <iostream>
#include <optional>
#include<stdexcept>

template <class Key>
class Node {
 public:
  Node() : data_(std::nullopt), left_(nullptr), right_(nullptr) {} //Constructs an empty node
  Node(const Key& data) : data_(data), left_(nullptr), right_(nullptr) {};   // Constructs a new Node by passing the data and sets the two child as null.
  Node(const Key& data, std::unique_ptr<Node<Key>>&& left, std::unique_ptr<Node<Key>>&& right) 
    : data_(data), left_(std::move(left)), right_(std::move(right)) {
      if (left_) left_->SetLevel(level_ + 1);
      if (right_) right_->SetLevel(level_ + 1);
    } // Constructs a new Node by passing the data and child nodes.
  
  Key GetData() const { // Returns the data stored in the node
    if (data_.has_value()) return data_.value();
    else throw std::invalid_argument("Node is empty");
  } 

  Node<Key>* GetLeftNode() const {return left_.get();} // Returns a pointer to the left node
  Node<Key>* GetRightNode() const {return right_.get();} // Returns a pointer to the right node
  
void SetLeftNode(std::unique_ptr<Node<Key>>&& left) {
  left_ = std::move(left);
  if (left_) {
    left_->SetLevel(level_ + 1);
    UpdateChildLevels(left_.get());
  }
} 

void SetRightNode(std::unique_ptr<Node<Key>>&& right) {
  right_ = std::move(right);
  if (right_) {
    right_->SetLevel(level_ + 1);
    UpdateChildLevels(right_.get());
  }
} 

  void SetLevel(const int& level) {level_ = level;}

  int GetLevel() const {return level_;} // A getter to acces the level
  bool Empty() const {
    return !data_.has_value(); 
  }
  
  friend std::ostream& operator<<(std::ostream& os, const Node<Key>& node) {
    if (node.data_.has_value()) {
      os << "[" << node.data_.value() << "]";
    } else {
      os << "[.]";
    }
    return os;
  }
  
 protected:
  void UpdateChildLevels(Node<Key>* node) {
    if (node == nullptr) return;
    if (node->left_) {
      node->left_->SetLevel(node->level_ + 1);
      UpdateChildLevels(node->left_.get());
    }
    if (node->right_) {
      node->right_->SetLevel(node->level_ + 1);
      UpdateChildLevels(node->right_.get());
    }
}
  int level_{0};
  std::optional<Key> data_;
  std::unique_ptr<Node<Key>> left_;
  std::unique_ptr<Node<Key>> right_;
};