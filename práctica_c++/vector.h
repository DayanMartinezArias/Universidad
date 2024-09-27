// Vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <stdexcept>

template<typename T>
class Vector {
public:
    Vector(const int size);
    Vector();
    ~Vector();

    T& at(const int index);
    const T& at(const int index) const;
    int GetSize() const;
    void Resize(const int new_size);
    void Reserve(const int new_capacity);
    void PushBack(const T& obj);
    T PopBack();
    void PushFront(const T& obj);
    T PopFront();
    void AllTo(const T& val);


    Vector& operator=(const Vector& other);

    template<typename U>
    friend std::ostream& operator<<(std::ostream& os, const Vector<U>& obj);
    template<typename U>
    friend std::istream& operator>>(std::istream& is, Vector<U>& obj);

private:
    void build(const int size);
    void destroy();

    T* vec_;
    int size_;
    int capacity_;
};

// Vector.cpp (inside header if using templates)

// Constructor
template<typename T>
Vector<T>::Vector(const int size) {
    build(size);
}

template<typename T>
Vector<T>::Vector() : size_(0), capacity_(1) {
  vec_ = new T[capacity_];  // Allocate memory
}

// Destructor
template<typename T>
Vector<T>::~Vector() {
    destroy();
}

// Build
template<typename T>
void Vector<T>::build(const int size) {
    size_ = size;
    capacity_ = size > 0 ? size : 1;
    vec_ = new T[capacity_];
}

// Destroy
template<typename T>
void Vector<T>::destroy() {
    delete[] vec_;
    vec_ = nullptr;
    size_ = 0;
    capacity_ = 0;
}

// Accessor (with bounds checking)
template<typename T>
T& Vector<T>::at(const int index) {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    return vec_[index];
}

template<typename T>
const T& Vector<T>::at(const int index) const {
    if (index < 0 || index >= size_) {
        throw std::out_of_range("Index out of bounds");
    }
    return vec_[index];
}


// Get Size
template<typename T>
int Vector<T>::GetSize() const {
    return size_;
}

template<typename T>
void Vector<T>::Reserve(const int new_capacity) {
  if (new_capacity > capacity_) {
    T* new_vec = new T[new_capacity];
    for (int i{0}; i < size_; ++i) {
      new_vec[i] = vec_[i];
    }
    delete[] vec_;
    vec_ = new_vec;
    capacity_ = new_capacity;
  }
}

// Resize
template<typename T>
void Vector<T>::Resize(const int new_size) {
    if (new_size > capacity_) {
        Reserve(new_size);  // Aumenta la capacidad si el nuevo tamaño lo requiere
    }
    size_ = new_size;  // Ajusta el tamaño
}




// Assignment operator
template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this == &other) {
        return *this;
    }
  
    destroy();
    build(other.size_);
    for (int i{0}; i < other.size_; ++i) {
        at(i) = other.at(i);
    }

    return *this;
}

// A method to insert a new element at the end of the vector
template<typename T>
void Vector<T>::PushBack(const T& obj) {
  if (size_ == capacity_) {
    Reserve(capacity_ * 2);  // Duplica la capacidad si es necesario
  }
  vec_[size_++] = obj;  // Añade el nuevo elemento y aumenta el tamaño
}

template<typename T>
T Vector<T>::PopBack() {
  T val{at(size_ - 1)};
  Resize(size_ - 1);
  return val;
}

template<typename T>
void Vector<T>::PushFront(const T& obj) {
  Vector<T> aux(size_ + 1);
  for (int i{0}; i < size_; ++i) {
    aux.at(i + 1) = at(i);
  }
  aux.at(0) = obj;
  *this = aux;
}

template<typename T>
T Vector<T>::PopFront() {
  T val =  at(0);
  Vector<T> aux(size_ - 1);
  for (int i{0}; i < aux.size_; ++i) {
    aux.at(i) = at(i + 1);
  }
  *this = aux;
  return val;
}

template<typename T>
void Vector<T>::AllTo(const T& val) {
  for(int i{0}; i < size_; ++i) {
    at(i) = val;
  }
}

// Overloading << operator
template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& obj) {
    os << "[";
    for (int i{0}; i < obj.size_; ++i) {
        os << obj.vec_[i];
        if (i < obj.size_ - 1) {
            os << ", ";
        }
    }
    os << "]";
    return os;
}

// Overloading >> operator
template<typename T>
std::istream& operator>>(std::istream& is, Vector<T>& obj) {
    for (int i{0}; i < obj.size_; ++i) {
        is >> obj.vec_[i];
    }
    return is;
}

#endif