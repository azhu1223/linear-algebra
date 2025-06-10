#include "MatrixGenerator.h"
#include "Matrix.h"
#include "Utility.h"

#include <memory>

template <int N, int M>
u_ptr<Matrix<N, M>> MatrixGenerator::generateMatrix(u_ptr<double[]> array) const {
    return u_ptr<Matrix<N, M>>(new Matrix<N, M>(std::move(array)));
}

template <int N, int M>
u_ptr<Matrix<N, M>> MatrixGenerator::generateMatrix() const {
    return generateMatrix<N, M>(u_ptr<double[]>(new double[N * M]));
}

template <int N>
u_ptr<Matrix<N, N>> MatrixGenerator::generateSquareMatrix() const {
    return generateMatrix<N, N>();
}

template <int N>
u_ptr<Matrix<N, N>> MatrixGenerator::generateIdentityMatrix() const {
    int size = N * N;
    u_ptr<double[]> matrixArray(new double[size]);

    int diagonalOffset = N + 1;

    for (int i = 0; i < size; i++) {
        // If the element is on the diagonal, place 1. Otherwise, place 0.
        matrixArray[i] = i % diagonalOffset == 0 ? 1 : 0;
    }

    return generateMatrix<N, N>(std::move(matrixArray));
}

template <int N>
u_ptr<Vector<N>> MatrixGenerator::generateVector(u_ptr<double[]> array) const {
    return u_ptr<Vector<N>>(new Vector<N>(std::move(array)));
}

template <int N>
u_ptr<Vector<N>> MatrixGenerator::generateVector() const {
    return generateVector<N>(u_ptr<double[]>(new double[N]));
}