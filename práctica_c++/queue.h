#ifndef QUEUE_H
#define QUEUE_H

#include "vector.h"

template <typename T>
class Queue {
 public:
   Queue();
   void PushBack(const T& element);
   T PopFront();
   bool Empty();
   T Front();
   T Back();
 private:
   Vector<T> vector_;
   int size_;
};

template<typename T>
Queue<T>::Queue() : size_(0) {};

template<typename T>
void Queue<T>::PushBack(const T& element) {
  vector_.PushBack(element);
  size_++;
}

template<typename T>
T Queue<T>::PopFront() {
  T val = vector_.PopFront();
  size_--;
  return val;
}

template<typename T>
bool Queue<T>::Empty() {
  return (size_ == 0);
}

template<typename T>
T Queue<T>::Front() {
  return vector_.at(0);
}

template<typename T>
T Queue<T>::Back() {
  return vector_.at(size_ - 1);
}

#endif



