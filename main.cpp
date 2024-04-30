#include <iostream>
#include "Matrix.hpp"
#include "matrix_v2.hpp"

#define MATRIX_SIZE 10

namespace matrix = matrix_v2;

int main(int argc, char const *argv[])
{
    // matrix::SparseArray<int, -1> sparse_array;
    // std::cout << "Sparse array size: " << sparse_array.size() << std::endl;
    // std::cout << "Sparse array [0] = " << sparse_array[0] << std::endl;
    // sparse_array[0] = 2;
    // std::cout << "Sparse array size: " << sparse_array.size() << std::endl;
    // std::cout << "Sparse array [0] = " << sparse_array[0] << std::endl;


    // matrix::SparseMatrix<int, -1> m;
    // std::cout << "Sparse matrix size: " << m.size() << std::endl;
    // std::cout << "Sparse matrix [0] = " << m[0][0] << std::endl;
    // m[0][0] = 2;
    // m[0][1];
    // std::cout << "Sparse matrix size: " << m.size() << std::endl;
    // std::cout << "Sparse matrix [0] = " << m[0][0] << std::endl;

    matrix::SparseMatrix<int, -1> m;
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        m[i][i] = i;
    }

    for (int i = 0; i < MATRIX_SIZE; ++i) {
        m[i][MATRIX_SIZE - i - 1] = MATRIX_SIZE - i -1;
    }

    // Выводим все строки
    for (int i = 1; i < 9; ++i)  {
        for (int j = 1; j < 9; ++j) {
            std::cout << m[i][j] << " ";
        }
        std::cout << std::endl;
    }

    // Выводим размер матрицы
    std::cout << "Sparse matrix size: " << m.size() << std::endl;

    for (auto el : m) {
        std::cout << "Element [" << el.row() << "][" << el.column() << "] =  " << el << std::endl;
    }

    return 0;
}
