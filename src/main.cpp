#include "Utility.h"
#include "Matrix.h"
#include "Vector.h"
#include "MatrixGenerator.h"
#include "LinearAlgebra.h"
#include "ThreadPool.h"

#include <iostream>
#include <random>
#include <functional>
#include <chrono>
#include <thread>

#define LEFT_ROWS 100
#define LEFT_COLS 100
#define RIGHT_ROWS LEFT_COLS
#define INTERNAL_DIMENSION LEFT_COLS
#define RIGHT_COLS 100

#define LEFT_SIZE LEFT_ROWS * LEFT_COLS
#define RIGHT_SIZE RIGHT_ROWS * RIGHT_COLS

//#define PRINT_MATRICES
#define TEST_SINGLETHREADED
#define TEST_MULTITHREADED

int main() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(-1.0, 1.0);

    auto randNum = std::bind(dis, gen);

    MatrixGenerator mg;

    u_ptr<double[]> leftArray = u_ptr<double[]>(new double[LEFT_SIZE]);
    u_ptr<double[]> rightArray = u_ptr<double[]>(new double[RIGHT_SIZE]);

    for (int i = 0; i < LEFT_SIZE; i++) {
        leftArray[i] = randNum();
    }

    for (int i = 0; i < RIGHT_SIZE; i++) {
        rightArray[i] = randNum();
    }

    u_ptr<Matrix<LEFT_ROWS, LEFT_COLS>> leftMatrix = mg.generateMatrix<LEFT_ROWS, LEFT_COLS>(std::move(leftArray));
    u_ptr<Matrix<RIGHT_ROWS, RIGHT_COLS>> rightMatrix = mg.generateMatrix<RIGHT_ROWS, RIGHT_COLS>(std::move(rightArray));

    #ifdef PRINT_MATRICES
    std::cout << "First matrix:\n";
    for (int i = 0; i < LEFT_ROWS; i++) {
        for (int j = 0; j < LEFT_COLS; j++) {
            std::cout << leftMatrix->get(i, j).second << ' ';
        }

        std::cout << '\n';
    }

    std::cout << '\n';

    std::cout << "Second matrix:\n";
    for (int i = 0; i < RIGHT_ROWS; i++) {
        for (int j = 0; j < RIGHT_COLS; j++) {
            std::cout << rightMatrix->get(i, j).second << ' ';
        }

        std::cout << '\n';
    }

    std::cout << '\n';
    #endif

    #ifdef TEST_SINGLETHREADED
    auto singleStart = std::chrono::steady_clock::now();
    u_ptr<Matrix<LEFT_ROWS, RIGHT_COLS>> singleThreadedProduct = SingleThreaded::product<LEFT_ROWS, LEFT_COLS, RIGHT_COLS>(mg, *leftMatrix, *rightMatrix).second;
    auto singleEnd = std::chrono::steady_clock::now();

    auto singleTimeElapsed = singleEnd - singleStart;

    #ifdef PRINT_MATRICES
    std::cout << "Singlethreaded matrix:\n";
    for (int i = 0; i < LEFT_ROWS; i++) {
        for (int j = 0; j < RIGHT_COLS; j++) {
            std::cout << singleThreadedProduct->get(i, j).second << ' ';
        }

        std::cout << '\n';
    }
    #endif
    
    std::cout << "Singlethreaded multiplication took ";
    std::cout << std::chrono::duration<double, std::milli>(singleTimeElapsed).count() << " ms.\n";

    #endif

    #ifdef TEST_MULTITHREADED

    std::cout << "Testing Multithreaded\n";

    ThreadPool tp;
    tp.start(std::thread::hardware_concurrency());

    auto multiStart = std::chrono::steady_clock::now();
    u_ptr<Matrix<LEFT_ROWS, RIGHT_COLS>> multiThreadedProduct = MultiThreaded::product<LEFT_ROWS, INTERNAL_DIMENSION, RIGHT_COLS>(mg, tp, *leftMatrix, *rightMatrix).second;
    auto multiEnd = std::chrono::steady_clock::now();
    auto multiTimeElapsed = multiEnd - multiStart;

    tp.stop();

    #ifdef PRINT_MATRICES
    std::cout << "Multithreaded matrix:\n";
    for (int i = 0; i < LEFT_ROWS; i++) {
        for (int j = 0; j < RIGHT_COLS; j++) {
            std::cout << multiThreadedProduct->get(i, j).second << ' ';
        }

        std::cout << '\n';
    }
    #endif

    std::cout << "Mulithreaded multiplication took ";
    std::cout << std::chrono::duration<double, std::milli>(multiTimeElapsed).count() << " ms.\n";

    #endif
}