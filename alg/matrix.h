#include <iostream>
#include <vector>
#include <expected>

template <class T>
class matrix_t {
public: 
    matrix_t() : rows_(0), columns_(0) {}
    matrix_t(size_t rows, size_t columns);
    matrix_t(size_t rows, size_t columns, const T& default_value);
    void Print() const;
    std::expected<T&, int> at(size_t row, size_t col);
    std::expected<const T&, int> at(size_t row, size_t col) const;
    bool IsSquare() const;

private:
    size_t rows_;
    size_t columns_;
    std::vector<T> matrix_;

    bool IsValidIndex(size_t row, size_t col) const;
};

template <class T>
bool matrix_t<T>::IsSquare() const {
    return rows_ == columns_;
}

template <class T>
matrix_t<T>::matrix_t(size_t rows, size_t columns) 
    : rows_(rows), columns_(columns) {
    if (rows == 0 || columns == 0) {
        throw std::invalid_argument("Rows and columns must be greater than 0");
    }
    matrix_.resize(rows * columns);
}

template <class T>
matrix_t<T>::matrix_t(size_t rows, size_t columns, const T& default_value) 
    : rows_(rows), columns_(columns) {
    if (rows == 0 || columns == 0) {
        throw std::invalid_argument("Rows and columns must be greater than 0");
    }
    matrix_.resize(rows * columns, default_value);
}

template <class T>
bool matrix_t<T>::IsValidIndex(size_t row, size_t col) const {
    return row >= 1 && row <= rows_ && col >= 1 && col <= columns_;
}

template <class T>
std::expected<T&, int> matrix_t<T>::at(size_t row, size_t col) {
    if (!IsValidIndex(row, col)) {
        return std::unexpected(-1); // Error code -1 indica índice inválido.
    }
    return matrix_[(row - 1) * columns_ + (col - 1)];
}

template <class T>
std::expected<const T&, int> matrix_t<T>::at(size_t row, size_t col) const {
    if (!IsValidIndex(row, col)) {
        return std::unexpected(-1); // Error code -1 indica índice inválido.
    }
    return matrix_[(row - 1) * columns_ + (col - 1)];
}

template <class T>
void matrix_t<T>::Print() const {
    for (size_t i = 1; i <= rows_; ++i) {
        for (size_t j = 1; j <= columns_; ++j) {
            auto value = at(i, j);
            if (value) {
                std::cout << value.value() << " ";
            } else {
                std::cout << "Error ";
            }
        }
        std::cout << "\n";
    }
}






