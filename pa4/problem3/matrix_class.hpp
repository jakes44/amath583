// AMATH 483-583 row major Matrix class template starter
// write the methods for:
// transpose
// infinityNorm
// operator*
// operator+

#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>

template <typename T>
class Matrix
{
public:
    Matrix(int numRows, int numCols)
        : num_rows(numRows), num_cols(numCols), data(numRows * numCols) {}

    T &operator()(int i, int j)
    {
        return data[i * num_cols + j];
    }

    const T &operator()(int i, int j) const
    {
        return data[i * num_cols + j];
    }

    Matrix<T> operator*(const Matrix<T> &other) const
    {
        // Make sure we can actually multiply
        // m x n * n x p = m x p
        if (num_cols != other.num_rows) {
            throw std::invalid_argument("Matrix dimensions do not match for multiplication.");
        }
        // Modified from problem 2 code
        Matrix<T> result(num_rows, other.num_cols);
        for (int i = 0; i < num_rows; i++){
            for (int j = 0; j < other.num_cols; j++){
                result(i, j) = 0;
                for (int k = 0; k < num_cols; k++){
                    result(i, j) += (*this)(i, k) * other(k, j);
                }
            }
        }

        return result;
    }

    Matrix<T> operator+(const Matrix<T> &other) const;

    Matrix<T> transpose() const
    {
        Matrix<T> transposed(num_cols, num_rows);
        for (int i = 0; i < num_cols; i++){
            for (int j = 0; j < num_rows; j++) {
                transposed(i, j) = (*this)(j, i);
            }
        }

        return transposed;
    }

    int numRows() const
    {
        return num_rows;
    }

    int numCols() const
    {
        return num_cols;
    }

    T infinityNorm() const
    {
        // Max of row sums
        T norm = 0;
        for(int i = 0; i < num_rows; i++){
            T row_sum = 0;
            for (int j = 0; j < num_cols; j++){
                row_sum += std::abs((*this)(i, j));
            }
            norm = std::max(norm, row_sum);
        }
        return norm;
    }

private:
    int num_rows;
    int num_cols;
    std::vector<T> data;
};

template <typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &other) const
{
    // check for errors in Matrix dimensions
    if ((*this).num_rows != other.num_rows || (*this).num_cols != other.num_cols){
        throw std::invalid_argument("Matrix dimensions do not match for addition.");
    }

    Matrix<T> result(num_rows, num_cols);
    for (int i = 0; i < num_rows; i++){
        for (int j = 0; j < num_cols; j++){
            result(i, j) = (*this)(i, j) + other(i, j);
        }
    }
    return result;
}
