#include "Matrix.h"
#include "Utility.h"

#include <memory>

template <int N, int M>
Matrix<N, M>::Matrix(u_ptr<double[]> matrix) : 
    m_matrixArray(std::move(matrix)), m_rows(N), m_cols(M)
{}

template <int N, int M>
Result<double> Matrix<N, M>::get(int n, int m) const {
    if (n < 0 || m < 0 || n >= m_rows || m >= m_cols) {
        return {false, 0.0};
    }

    return {true, m_matrixArray[index(n, m)]};
}

template <int N, int M>
bool Matrix<N, M>::set(int n, int m, double val) {
    if (n < 0 || m < 0 || n >= m_rows || m >= m_cols) {
        return false;
    }

    m_matrixArray[index(n, m)] = val;

    return true;
}

template <int N, int M>
int Matrix<N, M>::rows() const {
    return m_rows;
}

template <int N, int M>
int Matrix<N, M>::cols() const {
    return m_cols;
}