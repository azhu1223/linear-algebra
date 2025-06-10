#ifndef MATRIX_GENERATOR_H
#define MATRIX_GENERATOR_H

#include "Matrix.h"
#include "Vector.h"
#include "Utility.h"

class MatrixGenerator {
public:
    // Initialized from outside
    template <int N, int M>
    u_ptr<Matrix<N, M>> generateMatrix(u_ptr<double[]> array) const;

    // Uninitialized
    template <int N, int M>
    u_ptr<Matrix<N, M>> generateMatrix() const;

    template <int N>
    u_ptr<Matrix<N, N>> generateSquareMatrix() const;

    template <int N>
    u_ptr<Matrix<N, N>> generateIdentityMatrix() const;

    template <int N>
    u_ptr<Vector<N>> generateVector(u_ptr<double[]> array) const;

    template <int N>
    u_ptr<Vector<N>> generateVector() const;
};

#include "MatrixGenerator.tpp"

#endif
