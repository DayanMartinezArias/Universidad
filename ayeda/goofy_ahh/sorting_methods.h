#pragma once 
#include "static_sequence.h"
#include <iostream>

template <typename Key>
void Insertion(StaticSequence<Key>& seq, size_t size, bool trace) {
  for (size_t i = 1; i < size; ++i) {
    Key key = seq[i]; 
    int j = i - 1;
    while (j >= 0 && seq[j] > key) { 
      seq[j + 1] = seq[j];
      j--;
    }
    seq[j + 1] = key;

    if (trace) {
      std::cout << "After iteration: " << i << ": ";
      seq.PrintTable();
    }
  }
}

template <typename Key>
void Shake(StaticSequence<Key>& seq, size_t size, bool trace) {
  int start{1};
  int end = size - 1;
  int cam = size;
  while (start < end) {
    for (int j{end}; j >= start; --j) {
      if (seq[j] < seq[j - 1]) {
        seq.swap(j - 1, j);
        cam = j;
      }
    }
    start = cam + 1;
    if (trace) {
      std::cout << "After backward pass: ";
      seq.PrintTable();
    }

    for (int j = start; j <= end; ++j) {
      if (seq[j] < seq[j - 1]) {
        seq.swap(j - 1, j);
        cam = j;
      }
    }
    end = cam - 1;
    if (trace) {
      std::cout << "After forward pass: ";
      seq.PrintTable();
    }
  }
}

template <typename Key>
void QuickSort(StaticSequence<Key>& seq, size_t start_of_array, size_t size, bool trace) {
  int start{start_of_array}, end{start_of_array + size - 1};
  Key pivot{seq[(start + end) / 2]};
  while (start <= end) {
    while (seq[start] < pivot) start++;
    while (seq[end] > pivot) end--;
    if (start <= end) {
      seq.swap(start, end);
      start++, end--;
    }
  }
  if (trace) {
    std::cout << "After partition with pivot: " << pivot << "): ";
    seq.PrintTable();
  }
  if (start_of_array < end) QuickSort(seq, start_of_array, end - start_of_array + 1, trace);
  if (start < size) QuickSort(seq, start, (start_of_array + size) - start, trace);
}

template <typename Key>
void baja(int i, StaticSequence<Key>& seq, size_t size, bool trace) {
  int left, right, maxChild;
  while (true) {
    left = 2 * i + 1;
    right = 2 * i + 2;
    maxChild = i;

    if (left < size && seq[left] > seq[maxChild]) {
      maxChild = left;
    }
    if (right < size && seq[right] > seq[maxChild]) {
      maxChild = right;
    }
    if (maxChild == i) {
      break;
    }
    seq.swap(i, maxChild);
    i = maxChild;
  }
}

template <typename Key>
void HeapSort(StaticSequence<Key>& seq, size_t size, bool trace) {
  for (int i = size / 2 - 1; i >= 0; --i) {
    baja(i, seq, size, trace);
  }
  if (trace) {
    std::cout << "After creating the heap: ";
    seq.PrintTable();
  }

  for (int i = size - 1; i > 0; --i) {
    seq.swap(0, i);
    baja(0, seq, i, trace);
    if (trace) {
      std::cout << "After extraction from the heap number" << size - i << ": ";
      seq.PrintTable();
    }
  }
}

template <typename Key>
void deltasort(int delta, StaticSequence<Key>& sec, int n, bool trace) {
  for (int i = delta; i < n; ++i) {
    Key x = sec[i];
    int j = i;
    while (j >= delta && x < sec[j - delta]) {
      sec[j] = sec[j - delta];
      j -= delta;
    }
    sec[j] = x;
  }
  if (trace) {
    std::cout << "After delta value" << delta << ": ";
    sec.PrintTable();
  }
}

template <typename Key>
void ShellSort(StaticSequence<Key>& seq, int n, float alfa, bool trace) {
  int delta = n;
  while (delta > 1) {
    delta = static_cast<int>(delta * alfa);
    if (delta < 1) delta = 1;
    deltasort(delta, seq, n, trace);
    if (delta == 1) break; 
  }
}