#include "Vector.h"
#include "Matrix.h"

#include <memory>
#include <cmath>

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

template <int N>
double Vector<N>::normSquare() const {
    double acc = 0.0;

    for (int i = 0; i < N; i++) {
        double currentElement = get(i);
        acc += currentElement * currentElement;
    }

    return acc;
}

template <int N>
double Vector<N>::norm() const {
    return std::sqrt(normSquare());
}
