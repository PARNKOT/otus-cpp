#pragma once
#include <iostream>
#include <map>
#include <type_traits>
#include <tuple>

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

            const index_t row() const { return first_index_; }
            const index_t column() const { return second_index_; }

            private:
                map_t* parent_map_ = nullptr;
                index_t first_index_ = -1;
                index_t second_index_ = -1;
        };

        /*
            TODO: Fix Iterator! Неправильно выводятся значения элементов матрицы
        */
        struct Iterator {
            using iter_value = Value<T>;

            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = iter_value;
            using pointer = iter_value*;
            using reference = iter_value&;

            using  map_iter_type = typename map_t::iterator;

            explicit Iterator(map_t* map, map_iter_type p) : map_(map), iter_(p) {
                //size_ = p->size();
            }

            value_type operator*() {
                return value_type{map_, row_, column_};
            }

            // value_type operator->() {
            //     return 
            // }

            Iterator& operator++()  {
                if (++column_ == iter_->second.size()) {
                    ++row_;
                    ++iter_;
                    column_ = 0;
                }

                return *this;
            }

            friend bool operator== (const Iterator& a, const Iterator& b){
                return (a.iter_ == b.iter_) && (a.column_ == b.column_);
            }

            friend bool operator!= (const Iterator& a, const Iterator& b){
                return (a.iter_ != b.iter_) || (a.column_ != b.column_);
            }

            private:
                map_t* map_ = nullptr;
                map_iter_type iter_;
                index_t row_ = 0;
                index_t column_  = 0;
                std::size_t size_ = 0;

        };

        Iterator begin() {
            return Iterator{&matrix_, matrix_.begin()};
        }

        Iterator end() {
            return Iterator{&matrix_, matrix_.end()};
        }

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
