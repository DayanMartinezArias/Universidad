#pragma once 
#include "dispersion_function.h"

template <class Key>
class ExplorationFunction {
 public:
  virtual unsigned operator()(const Key& k, unsigned i) const = 0;
  virtual ~ExplorationFunction() {}
};

template <class Key>
class LinearExploration : public ExplorationFunction<Key> {
public:
 unsigned operator()(const Key& k, unsigned i) const override {
   return i; 
 }
};

template <class Key>
class QuadraticExploration : public ExplorationFunction<Key> {
 public:
  unsigned operator()(const Key& k, unsigned i) const override {
    return i * i;  
  }
};

template <class Key>
class DoubleHashingExploration : public ExplorationFunction<Key> {
 public:
  DoubleHashingExploration(DispersionFunction<Key>& f) : secondHashFunction(f) {}
  unsigned operator()(const Key& k, unsigned i) const override {
    return secondHashFunction(k) * i;  // Desplazamiento usando doble dispersión
  }
 private:
  DispersionFunction<Key>& secondHashFunction;  // Segunda función de dispersión
};

template <class Key>
class RandomExploration : public ExplorationFunction<Key> {
 public:
  unsigned operator()(const Key& k, unsigned i) const override {
    std::srand(k);  // Inicializa la semilla con la clave
    return std::rand();  // Desplazamiento pseudoaleatorio
  }
};