#pragma once
#include <iostream>
#include <map>
#include <type_traits>
#include <tuple>

namespace matrix_v1 {
    using index_t = std::size_t;

    template<typename  T, T Default>
    struct SparseMatrix {
        struct Element {
            index_t row = 0;
            index_t column = 0;
            T value;

            operator std::tuple<index_t&, index_t&, T&>() {
                return std::tuple<index_t&, index_t&, T&>(row, column, value);
            }
        };

        using column_map_t = std::map<index_t, Element>;
        using row_map_t = std::map<index_t, column_map_t>;

        template <typename value_t>
        struct ValueWrapper {
            explicit ValueWrapper(row_map_t* p_map, index_t first_index, index_t second_index) : 
            parent_map_{p_map}, first_index_{first_index}, second_index_{second_index} {}

            ValueWrapper& operator =(const T& value) {
                if (parent_map_->count(first_index_) == 0) {
                    (*parent_map_)[first_index_] = column_map_t();
                }

                Element _value = {first_index_, second_index_, value};
                parent_map_->at(first_index_)[second_index_] = _value;

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

                return parent_map_->at(first_index_).at(second_index_).value;
            };

            template <typename Fake = std::enable_if<std::is_same<value_t, ValueWrapper<T>>::value>>
            value_t operator [](index_t index) {
                return value_t{parent_map_, first_index_, index};
            }

            const index_t row() const { return first_index_; }
            const index_t column() const { return second_index_; }

            private:
                row_map_t* parent_map_ = nullptr;
                index_t first_index_ = -1;
                index_t second_index_ = -1;
        };

        struct Iterator {
            using iter_value = Element;

            using iterator_category = std::forward_iterator_tag;
            using difference_type = std::ptrdiff_t;
            using value_type = iter_value;
            using pointer = iter_value*;
            using reference = iter_value&;

            using column_map_iter_t = typename column_map_t::iterator;
            using row_map_iter_t = typename row_map_t::iterator;

            explicit Iterator(row_map_t* map, row_map_iter_t iter) : map_(map), row_iter_(iter),
                                                                     column_iter_((*iter).second.begin()) {}

            value_type operator*() {
                return (*column_iter_).second;
            }

            Iterator& operator++()  {
                if (++column_iter_ == (*row_iter_).second.end()) {
                    ++row_iter_;
                    column_iter_ = (*row_iter_).second.begin();
                }

                return *this;
            }

            friend bool operator== (const Iterator& a, const Iterator& b){
                return (a.row_iter_ == b.row_iter_) && (a.column_iter_ == b.column_iter_);
            }

            friend bool operator!= (const Iterator& a, const Iterator& b){
                return (a.row_iter_ != b.row_iter_) || (a.column_iter_ != b.column_iter_);
            }

            private:
                row_map_t* map_ = nullptr;
                column_map_iter_t column_iter_;
                row_map_iter_t row_iter_;
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

        ValueWrapper<ValueWrapper<T>> operator [](index_t index) {
            return ValueWrapper<ValueWrapper<T>>{&matrix_, index, 0};
        }
    private:
        row_map_t matrix_;
    };

}
