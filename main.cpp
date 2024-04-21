#include <iostream>
#include "Matrix.hpp"


int main(int argc, char const *argv[])
{
    matrix::SparseArray<int, -1> sparse_array;
    std::cout << "Sparse array size: " << sparse_array.size() << std::endl;
    std::cout << "Sparse array [0] = " << sparse_array[0] << std::endl;
    sparse_array[0] = 2;
    std::cout << "Sparse array size: " << sparse_array.size() << std::endl;
    std::cout << "Sparse array [0] = " << sparse_array[0] << std::endl;

    return 0;
}
