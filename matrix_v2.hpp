#pragma once
#include <iostream>
#include <map>
#include <type_traits>

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

            T& operator [](index_t index) {
                if (map_->count(index_) == 0) {
                    return Default;
                }
                return map_->at(index_);
            }

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
        using map_t = std::map<index_t, std::map<index_t, T>>;

        template <typename value_t>
        struct Value {
            explicit Value(map_t* p_map, index_t first_index, index_t second_index) : 
            parent_map_{p_map}, first_index_{first_index}, second_index_{second_index} {}

            Value& operator =(const T& value) {
                if (parent_map_->count(first_index_) == 0) {
                    (*parent_map_)[first_index_] = std::map<index_t, T>();
                }

                parent_map_->at(first_index_)[second_index_] = value;

                return *this;
            }

            template <typename Fake = std::enable_if<std::is_same<value_t, T>::value>>
            operator value_t() {
                if (parent_map_->count(first_index_)  == 0) {
                    return Default;
                }

                if (parent_map_->at(first_index_).count(second_index_) == 0) {
                    return Default;
                }

                return parent_map_->at(first_index_).at(second_index_);
            };

            template <typename Fake = std::enable_if<std::is_same<value_t, Value<T>>::value>>
            value_t operator [](index_t index) {
                return value_t{parent_map_, first_index_, index};
            }

            private:
                map_t* parent_map_ = nullptr;
                index_t first_index_ = -1;
                index_t second_index_ = -1;
        };

        constexpr std::size_t size() const noexcept {
            std::size_t res{0};
            for (const auto& p : matrix_){
                res += p.second.size();
            }

            return res;
        }

        Value<Value<T>> operator [](index_t index) {
            return Value<Value<T>>{&matrix_, index, 0};
        }

    private:
        std::map<index_t, std::map<index_t, T>> matrix_;
    };

}
