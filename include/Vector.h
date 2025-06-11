#ifndef VECTOR_H
#define VECTOR_H

#include "Matrix.h"

template <int N>
class Vector : public Matrix<N, 1> {
public:
    Result<double> get(int n) const;
    bool set(int n, double val);
    int size() const;
    double normSquare() const;
    double norm() const;

    friend class MatrixGenerator;
protected:
    Vector(u_ptr<double[]> vector);
private:
};

using Vector2 = Vector<2>;
using Vector3 = Vector<3>;

#include "Vector.tpp"

#endif
