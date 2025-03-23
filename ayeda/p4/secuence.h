#pragma once
#include <vector>

template <class Key>
class Sequence {
 public:
  virtual bool search(const Key& k) const = 0;
  virtual bool insert(const Key& k) = 0;
  virtual ~Sequence() {};
};

template <class Key> 
class dynamicSequence: public Sequence<Key> {
 public:
  bool search(const Key& k) const override {
    for (const auto& item : data_) {
      if (item == k) return true;
    }
    return false;
  }
  bool insert(const Key& k) {
    data_.emplace_back(k);
    return true;
  }
  private:
   std::vector<Key> data_;
};

template<class Key> 
class staticSequence: public Sequence<Key> {
 public:
  staticSequence(unsigned size) : sz_(size) {
    data_.resize(sz_);
  }
  bool search(const Key& k) const override {
    for (const auto& item : data_) {
      if (item == k) return true;
    }
   return false;
  }

  bool insert(const Key& k) const override {
   for (auto& item : data_) {
     if (item == Key()) {
      item = k;
      return true;
     }
   }
   return false;
  }

  bool isFull() const {
    for (const auto& item : data_) {
      if (item == Key()) return false;
    }
    return true;
  }
  private:
   std::vector<Key> data_;  // Estructura estática para almacenar claves
   unsigned sz_;     // Tamaño máximo del bloque
};

