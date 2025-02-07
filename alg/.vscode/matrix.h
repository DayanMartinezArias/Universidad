#include <iostream>
#include <vector>

template <class T>
class matrix_t {
  public: 
   matrix_t() = default;
   matrix_t(const size_t& columns, const size_t& rows);
   matrix_t(const size_t& columns, const size_t& rows, const T& default_value);
   void Print() const;
   T& at(const size_t& row, const size_t& col);
   const T& at(const size_t& row, const size_t& col) const;

  private:
   size_t columns_;
   size_t rows_;
   std::vector<T> matrix_;
};

template <class T>
matrix_t<T>::matrix_t(const size_t& columns, const size_t& rows) : columns_(columns), rows_(rows) {
  matrix_.resize(rows * columns);
}

template <class T>
matrix_t<T>::matrix_t(const size_t& columns, const size_t& rows, const T& default_value) : columns_(columns), rows_(rows) {
  matrix_.resize(rows * columns, default_value);
}

template <class T>
T& matrix_t<T>::at(const size_t& row, const size_t& col) {
  return matrix_.at(columns_ * (row -1) + (col - 1))
}

template <class T>
const T& matrix_t<T>::at(const size_t& row, const size_t& col) const {
  return matrix_.at(columns_ * (row -1) + (col - 1))
}

template <class T>
void matrix_t<T>::Print() const {
  for (size_t i{1}, i <= columns_; ++i) {
    for (size_t j{1}, i <= rows_; ++i) {
      std::cout << at(i, j);
    }
  }
}



