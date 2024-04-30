#include <iostream>
#include "Matrix.hpp"
#include "matrix_v2.hpp"

namespace matrix = matrix_v2;

int main(int argc, char const *argv[])
{
    // matrix::SparseArray<int, -1> sparse_array;
    // std::cout << "Sparse array size: " << sparse_array.size() << std::endl;
    // std::cout << "Sparse array [0] = " << sparse_array[0] << std::endl;
    // sparse_array[0] = 2;
    // std::cout << "Sparse array size: " << sparse_array.size() << std::endl;
    // std::cout << "Sparse array [0] = " << sparse_array[0] << std::endl;


    matrix::SparseMatrix<int, -1> m;
    std::cout << "Sparse matrix size: " << m.size() << std::endl;
    std::cout << "Sparse matrix [0] = " << m[0][0] << std::endl;
    m[0][0] = 2;
    m[0][1];
    std::cout << "Sparse matrix size: " << m.size() << std::endl;
    std::cout << "Sparse matrix [0] = " << m[0][0] << std::endl;


    return 0;
}
