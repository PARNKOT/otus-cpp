#pragma once
#include <iostream>
#include <map>

namespace matrix {
    using index_t = std::size_t;


    template<typename  T, T Default>
    struct SparseArray {
        // TODO: fix size method. Do via loop and check if Default
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
    struct SparseMatrix {
        constexpr std::size_t size() const noexcept {
            std::size_t value = 0;
            for (const auto& p : matrix_)
                value += p.second.size();

            return value;
        }

        SparseArray<T, Default>& operator[](index_t index) {
            if (matrix_.count(index) == 0) {
                matrix_[index] = SparseArray<T, Default>();
            }
            return matrix_[index];
        }

    private:
        //SparseArray<SparseArray<T, Default>, Default> matrix_;
        std::map<index_t, SparseArray<T, Default>> matrix_;
    };

}
