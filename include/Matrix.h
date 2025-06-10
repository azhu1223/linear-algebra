#ifndef MATRIX_H
#define MATRIX_H

#include "Utility.h"

template<int N, int M>
class Matrix {
public:
    virtual Result<double> get(int n, int m) const;
    virtual bool set(int n, int m, double val);
    virtual int rows() const;
    virtual int cols() const;

    bool operator==(const Matrix& other) const;

    friend class MatrixGenerator;
protected:
    Matrix(u_ptr<double[]> matrix);
private:
    int index(int n, int m) const;

    int m_rows; 
    int m_cols;
    u_ptr<double[]> m_matrixArray;
};

template <int N, int M>
inline
int Matrix<N, M>::index(int n, int m) const {
    return n * m_cols + m;
}

template <int N, int M>
inline
bool Matrix<N, M>::operator==(const Matrix& other) const {
    if (this->m_rows != other.m_rows || this->m_cols != other.m_cols) {
        return false;
    }

    int size = m_rows * m_cols;

    for (int i = 0; i < size; i++) {
        if (this->m_matrixArray[i] != other.m_matrixArray[i]) {
            return false;
        }
    }

    return true;
}

#include "Matrix.tpp"

#endif