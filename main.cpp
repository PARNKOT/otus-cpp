#include <iostream>
#include "Matrix_v1.hpp"

#define MATRIX_SIZE 10

namespace matrix = matrix_v1;

int main(int argc, char const *argv[])
{
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

    //((m[9][9] = 10) = 15) = 20;

    // Выводим размер матрицы
    std::cout << "Sparse matrix size: " << m.size() << std::endl;

    // Итерируемся по элементам матрицы
    for (auto el : m) {
        matrix::index_t row;
        matrix::index_t column;
        int value;

        std::tie(row,  column, value) = el;

        std::cout << "Element [" << row << "][" << column << "] =  " << value << std::endl;
    }

    return 0;
}
