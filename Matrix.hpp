#pragma once
#include <iostream>
#include <map>

namespace matrix {
    using index_t = std::size_t;


    template<typename  T, T Default>
    struct SparseArray {
        constexpr std::size_t size() const noexcept {
            return array_.size();
        }

        T& operator[](index_t index) {
            if (array_.count(index) == 0) {
                array_[index] = Default;
            }
            return array_[index];
        }


    private:
        std::map<index_t, T> array_;
    };

    template<typename  T, T Default>
    struct Matrix {
        constexpr std::size_t size() const noexcept;

        T& operator[](index_t index) {
            
        }

    private:
        //SparseArray<SparseArray<T, Default>, Default> matrix_;
        std::map<index_t, SparseArray<T, Default>> matrix_;
    };

}
