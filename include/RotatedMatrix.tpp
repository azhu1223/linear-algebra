#include "RotatedMatrix.h"

template <int N, int M>
Result<double> RotatedMatrix<N, M>::get(int n, int m) const {
    return Matrix<M, N>::get(m, n);
}

template <int N, int M>
int RotatedMatrix<N, M>::rows() const {
    
}
template <int N, int M>
int RotatedMatrix<N, M>::cols() const;