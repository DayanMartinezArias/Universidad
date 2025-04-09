#pragma once
#include <vector>
#include <utility>

// Clase abstracta para definir la tabla
template <class Key>
class Sequence {
 public:
  virtual ~Sequence() {};
  virtual Key operator[](const unsigned value) const = 0;
  virtual Key& operator[](const unsigned value) = 0;
};

// Clase que define una tabla de tamaño fijo
template <class Key>
class StaticSequence : public Sequence<Key> {
 public:
  StaticSequence(const unsigned size) {sequence_.resize(size);}
  int get_size() const {return sequence_.size();}
  Key operator[](const unsigned value) const override;
  Key& operator[](const unsigned value) override;
  void swap(const unsigned value1, const unsigned value2) {std::swap(sequence_[value1], sequence_[value2]);}
  void PrintTable() const;
 private:
  std::vector<Key> sequence_;
};

template <class Key>
Key StaticSequence<Key>::operator[](const unsigned value) const {
  if (value >= sequence_.size() || value < 0) throw std::invalid_argument("Invalid acces to position on sequence");
  return sequence_[value];
}

template <class Key>
Key& StaticSequence<Key>::operator[](const unsigned value) {
  if (value >= sequence_.size() || value < 0) throw std::invalid_argument("Invalid acces to position on sequence");
  return sequence_[value];
}

template <class Key>
void StaticSequence<Key>::PrintTable() const {
  std::cout << "[";
  for (int i{0}; i < sequence_.size(); ++i) {
    if (sequence_.size() - i == 1) {
      std::cout << sequence_[i];
      continue;
    }
    std::cout << sequence_[i] << " ";
  }
  std::cout << "]" << std::endl;
}
