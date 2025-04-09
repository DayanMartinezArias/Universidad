#pragma once
#include "static_sequence.h"
#include "sorting_methods.h"
#include <iostream>

// Clase base abstracta para métodos de ordenación
template <class Key>
class SortMethod {
 public:
   SortMethod(StaticSequence<Key>& sequence, bool trace) : sequence_(sequence), trace_(trace) {}
   virtual ~SortMethod() {}
   virtual void Sort() = 0;

 protected:
  StaticSequence<Key>& sequence_;
  bool trace_;
};

// Clase para ordenación por inserción
template <class Key>
class InsertionSortMethod : public SortMethod<Key> {
 public:
  InsertionSortMethod(StaticSequence<Key>& sequence, bool trace) : SortMethod<Key>(sequence, trace) {}

  void Sort() override {
    Insertion(this->sequence_, this->sequence_.get_size(), this->trace_);
  }
};

// Clase para ordenación de la sacudida
template <class Key>
class ShakeSortMethod : public SortMethod<Key> {
 public:
  ShakeSortMethod(StaticSequence<Key>& sequence, bool trace) : SortMethod<Key>(sequence, trace) {}

  void Sort() override {
    Shake(this->sequence_, this->sequence_.get_size(), this->trace_);
  }
};

// Clase para ordenación QuickSort
template <class Key>
class QuickSortMethod : public SortMethod<Key> {
 public:
  QuickSortMethod(StaticSequence<Key>& sequence, bool trace) : SortMethod<Key>(sequence, trace) {}

  void Sort() override {
    QuickSort(this->sequence_, 0, this->sequence_.get_size(), this->trace_);
  }
};

// Clase para ordenación HeapSort
template <class Key>
class HeapSortMethod : public SortMethod<Key> {
 public:
  HeapSortMethod(StaticSequence<Key>& sequence, bool trace) : SortMethod<Key>(sequence, trace) {}

  void Sort() override {
    HeapSort(this->sequence_, this->sequence_.get_size(), this->trace_);
  }
};

// Clase para ordenación ShellSort
template <class Key>
class ShellSortMethod : public SortMethod<Key> {
 public:
  ShellSortMethod(StaticSequence<Key>& sequence, float alfa, bool trace) : SortMethod<Key>(sequence, trace), alfa_(alfa) {}

  void Sort() override {
    ShellSort(this->sequence_, this->sequence_.get_size(), alfa_, this->trace_);
  }

 private:
  float alfa_;
};