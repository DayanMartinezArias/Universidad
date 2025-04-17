#pragma once
#include <memory>
#include <iostream>
#include <optional>
#include <stdexcept>

template <class Key>
class Node {
 public:
  // Constructors
  Node();
  Node(const Key& data, const int& level = 0);
  Node(const Key& data, std::unique_ptr<Node<Key>>&& left, std::unique_ptr<Node<Key>>&& right, const int& level = 0);
    
  // Getters 
  Key GetData() const;
  Node<Key>* GetLeftNode() const;
  Node<Key>* GetRightNode() const;
  int GetLevel() const;
  bool Empty() const;
    
  // Setters
  void SetLeftNode(std::unique_ptr<Node<Key>>&& left);
  void SetRightNode(std::unique_ptr<Node<Key>>&& right);
  void SetLevel(const int& level);
  
  // Insertion operator
  friend std::ostream& operator<<(std::ostream& os, const Node<Key>& node) {
    if (node.data_.has_value()) {
      os << "[" << node.data_.value() << "]";
    } else {
      os << "[.]";
    }
    return os;
  }
  
 protected:
  void UpdateChildLevels(Node<Key>* node);

  int level_{0}; // Level of the node
  std::optional<Key> data_; // Data stored in the node
  std::unique_ptr<Node<Key>> left_; // Pointer to the child left node
  std::unique_ptr<Node<Key>> right_; // pointer to the child right node
}; 

/**
 * @brief Constructs a new empty node. It doesnt even holds a Key
 * 
 * @tparam Key 
 */
template <class Key>
Node<Key>::Node() : data_(std::nullopt), left_(nullptr), right_(nullptr), level_(0) {}

/**
 * @brief Construct a new node with just a key value and an optional level param
 * 
 * @tparam Key 
 * @param data 
 * @param level 
 */
template <class Key>
Node<Key>::Node(const Key& data, const int& level) : data_(data), left_(nullptr), right_(nullptr), level_(level) {}

/**
 * @brief Constructs a new node with key value, the optional level param and its two child nodes
 * 
 * @tparam Key 
 * @param data 
 * @param left 
 * @param right 
 * @param level 
 */
template <class Key>
Node<Key>::Node(const Key& data, std::unique_ptr<Node<Key>>&& left, std::unique_ptr<Node<Key>>&& right, const int& level)
  : data_(data), left_(std::move(left)), right_(std::move(right)), level_(level) {
  if (left_) left_->SetLevel(level_ + 1);
  if (right_) right_->SetLevel(level_ + 1);
}

/**
 * @brief Returns the Key value stored in the node
 * 
 * @tparam Key 
 * @return Key 
 */
template <class Key>
Key Node<Key>::GetData() const {
  if (data_.has_value()) return data_.value();
  else throw std::invalid_argument("Node is empty");
}

/**
 * @brief Returns a pointer to the left node
 * 
 * @tparam Key 
 * @return Node<Key>* 
 */
template <class Key>
Node<Key>* Node<Key>::GetLeftNode() const {
  return left_.get();
}

/**
 * @brief Returns a pointer to the right node
 * 
 * @tparam Key 
 * @return Node<Key>* 
 */
template <class Key>
Node<Key>* Node<Key>::GetRightNode() const {
  return right_.get();
}

/**
 * @brief Returns the level of the current node, useful when using trees
 * 
 * @tparam Key 
 * @return int 
 */
template <class Key>
int Node<Key>::GetLevel() const {
  return level_;
}

/**
 * @brief Returns true if the node is empty
 * 
 * @tparam Key 
 * @return true 
 * @return false 
 */
template <class Key>
bool Node<Key>::Empty() const {
  return !data_.has_value();
}

/**
 * @brief Sets the left node of the current node passing a pointer to the node
 * 
 * @tparam Key 
 * @param left 
 */
template <class Key>
void Node<Key>::SetLeftNode(std::unique_ptr<Node<Key>>&& left) {
  left_ = std::move(left);
  if (left_) {
    left_->SetLevel(level_ + 1);
    UpdateChildLevels(left_.get());
  }
}

/**
 * @brief Sets the left node of the current node passing a pointer to the node
 * 
 * @tparam Key 
 * @param right 
 */
template <class Key>
void Node<Key>::SetRightNode(std::unique_ptr<Node<Key>>&& right) {
  right_ = std::move(right);
  if (right_) {
    right_->SetLevel(level_ + 1);
    UpdateChildLevels(right_.get());
  }
}


/**
 * @brief Sets the level of the current node
 * 
 * @tparam Key 
 * @param level 
 */
template <class Key>
void Node<Key>::SetLevel(const int& level) {
  level_ = level;
}

/**
 * @brief Updates the level every child node recursively to adjust to the level of the parent
 * 
 * @tparam Key 
 * @param node 
 */
template <class Key>
void Node<Key>::UpdateChildLevels(Node<Key>* node) {
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
