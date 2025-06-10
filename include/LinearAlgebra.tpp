#include "LinearAlgebra.h"
#include "Utility.h"
#include "Matrix.h"
#include "MatrixGenerator.h"
#include "ThreadPool.h"

#include <thread>
#include <functional>

namespace SingleThreaded {
    template<int N, int M, int P>
    Result<u_ptr<Matrix<N ,P>>> product(const MatrixGenerator& mg, const Matrix<N, M>& left, const Matrix<M, P>& right) {
        int leftRows = left.rows();
        int leftCols = left.cols();
        int rightRows = right.rows();
        int rightCols = right.cols();

        if (leftCols != rightRows) {
            return {false, nullptr};
        }

        int productSize = leftRows * rightCols;
        u_ptr<double[]> productArray(new double[productSize]);

        double* rowArray = new double[leftCols];

        for (int i = 0; i < leftRows; i++) {
            int row = i * rightCols;
            
            for (int j = 0; j < leftCols; j++) {
                rowArray[j] = left.get(i, j).second;
            }

            for (int j = 0; j < rightCols; j++) {
                double acc = 0.0;

                for (int k = 0; k < leftCols; k++) {
                    acc += rowArray[k] * right.get(k, j).second;
                }

                productArray[row + j] = acc;
            }
        }

        delete[] rowArray;


        return {true, mg.generateMatrix<N, P>(std::move(productArray))};
    }

    template<int N, int M>
    Result<u_ptr<Matrix<N, M>>> sum(const MatrixGenerator& mg, const Matrix<N, M>& left, const Matrix<N, M>& right) {
        int rows = left.rows();
        int cols = left.cols();

        if (rows != right.rows() || cols != right.cols()) {
            return {false, nullptr};
        }

        u_ptr<double[]> sumArray(new double[rows * cols]);

        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                sumArray[i * cols + j] = left.get(i, j).second + right.get(i, j).second;
            }
        }

        return {true, mg.generateMatrix<N, M>(std::move(sumArray))};
    }
}

template<int N, int M, int P>
static void threadWork(double* productArray, const Matrix<N, M>& left, const Matrix<M, P>& right, int start, int end) {
    const int productCols = right.cols();

    const int internalSideSize = left.cols();

    const int startLeftRow = start / productCols;
    const int endLeftRow = (end - 1) / productCols;
    const int numRows = endLeftRow - startLeftRow + 1;

    u_ptr<double[]> leftRows(new double[numRows * internalSideSize]);

    for (int i = start; i < end; i++) {
        int row = i / productCols;
        int col = i % productCols;

        double acc = 0.0;
        for (int i = 0; i < internalSideSize; i++) {
            acc += left.get(row, i).second * right.get(i, col).second;
        }

        productArray[i] = acc;
    }
}

#include <iostream>
template<int N, int M, int P>
static void addTasks(double* productArray, int arrSize, ThreadPool& tp, const Matrix<N, M>& left, const Matrix<M, P>& right) {
    int maxThreads = std::thread::hardware_concurrency();

    int jobs = maxThreads;

    if (arrSize < maxThreads) {
        jobs = arrSize;
    }

    int jobSize = arrSize / jobs;
    int numJobsExtra = arrSize % jobs;

    for (int acc = 0, i = 0; i < jobs; i++, acc) {
        int workEnd = acc + jobSize;

        if (numJobsExtra > 0) {
            workEnd++;
            numJobsExtra--;
        }
        
        tp.queueJob( 
            [=, &left, &right] () {
                threadWork(productArray, left, right, acc, workEnd);
            }
        );

        acc = workEnd;
    }
}

namespace MultiThreaded {
    template<int N, int M, int P>
    Result<u_ptr<Matrix<N, M>>> product(const MatrixGenerator& mg, ThreadPool& tp, const Matrix<N, M>& left, const Matrix<M, P>& right) {
        if (left.cols() != right.rows()) {
            return {false, nullptr};
        }

        const int productRows = left.rows();
        const int productCols = right.cols();
        const int productSize = productRows * productCols;

        u_ptr<double[]> productArray(new double[productSize]);

        tp.addTasksAndWaitUntilCompleted(
            [&, productSize] () {
                addTasks<N, M, P>(productArray.get(), productSize, tp, left, right);
        });

        //addTasks(productArray.get(), productSize, tp, left, right);

        //std::cout << "Waiting for jobs to finish.\n";

        return {true, mg.generateMatrix<N, P>(std::move(productArray))};
    }

    /*Result<u_ptr<Matrix>> sum(const MatrixGenerator& mg, ThreadPool& tp, const Matrix& left, const Matrix& right) {

    }*/
}