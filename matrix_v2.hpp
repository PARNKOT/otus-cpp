#pragma once
#include <iostream>
#include <map>

namespace matrix_v2 {
    using index_t = std::size_t;


    template<typename  T, T Default>
    struct SparseArray {
        using map_t = std::map<index_t, T>;

        struct Value {
            explicit Value(map_t* p_map, index_t index) : map_{p_map},  index_{index} {}

            Value& operator =(const T& value) {
                (*map_)[index_] = value;
                return *this;
            }

            operator T() {
                if (map_->count(index_) == 0) {
                    return Default;
                }
                return map_->at(index_);
            };

            private:
                //T* value_ = Default;
                map_t* map_;
                index_t index_;
        };

        // TODO: fix size method. Do via loop and check if Default
        constexpr std::size_t size() const noexcept {
            return array_.size();
        }

        Value operator[](index_t index) {
            return Value(&array_, index);
        }


    private:
        map_t array_;
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
