#include "Vector.h"
#include "Matrix.h"

#include <memory>

template <int N>
Vector<N>::Vector(u_ptr<double[]> vector) : 
    Matrix<N, 1>(std::move(vector)) 
{}

template <int N>
Result<double> Vector<N>::get(int n) const {
    return Matrix<N, 1>::get(n, 1);
}

template <int N>
bool Vector<N>::set(int n, double val) {
    return Matrix<N, 1>::set(n, 1, val);
}

template <int N>
int Vector<N>::size() const {
    return Matrix<N, 1>::rows();
}