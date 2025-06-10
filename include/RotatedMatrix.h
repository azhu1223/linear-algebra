#ifndef ROTATED_MATRIX_H
#define ROTATED_MATRIX_H

#include "Matrix.h"

template <int N, int M>
class RotatedMatrix : public Matrix<M, N> {
    virtual Result<double> get(int n, int m) const;
    virtual int rows() const;
    virtual int cols() const;
};

#include "RotatedMatrix.tpp"

#endif
