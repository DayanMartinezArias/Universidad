#pragma once
#include <cstdlib>

template <class Key>
class DispersionFunction {
 public:
  virtual unsigned operator()(const Key& k) const = 0;
  virtual ~DispersionFunction() {}
};

template <class Key>
class ModuloDispersion : public DispersionFunction<Key> {
 public:
  ModuloDispersion(unsigned size) : tableSize(size) {}
  unsigned operator()(const Key& k) const override {
    return k % tableSize;
  }
 private:
  unsigned tableSize;
};

template <class Key>
class SumaDispersion : public DispersionFunction<Key> {
 public:
  SumaDispersion(unsigned size) : tableSize(size) {}
  unsigned operator()(const Key& k) const override {
    unsigned sum{0};
    for (const auto& inx : k) sum += inx;
    return sum % tableSize;
  }
 private:
  unsigned tableSize;
};

template <class Key>
class PseudoRandomDispersion : public DispersionFunction<Key> {
 public:
  PseudoRandomDispersion(unsigned size) : tableSize(size) {}
  unsigned operator()(const Key& k) const override {
    std::srand(k);
    return std::rand() % tableSize;
  }
 private:
  unsigned tableSize;
};