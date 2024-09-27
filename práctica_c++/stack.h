#ifndef STACK_H
#define STACK_H

#include "vector.h"

template <typename T>
class Stack {
 public:
  Stack() : size_(0) {};  // Initialize size_ to 0 in the constructor

  void Push(const T& value);
  T Pop();
  T Top() const;  // Marked as const since it doesn’t modify the stack
  bool Empty() const;  // Marked as const since it doesn’t modify the stack

 private:
  Vector<T> vector_;
  int size_;
};

template <typename T> 
void Stack<T>::Push(const T& value) {
  vector_.PushBack(value);
  size_++;
}

template <typename T>
T Stack<T>::Pop() {
  if (Empty()) {
    throw std::runtime_error("Pop from an empty stack");
  }
  T val = vector_.PopBack();
  size_--;
  return val;
}

template <typename T>
T Stack<T>::Top() const {
  if (Empty()) {
    throw std::runtime_error("Top from an empty stack");
  }
  return vector_.at(size_ - 1);
}

template <typename T>
bool Stack<T>::Empty() const {
  return size_ == 0;
}

#endif
