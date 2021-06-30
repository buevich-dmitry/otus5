#pragma once
#include <map>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cassert>

template <class T, T DefaultValue = 0>
class Matrix {
private:
    using ValuesMap = std::map<std::pair<size_t, size_t>, T>;
    using ValuesMapIterator = typename ValuesMap::const_iterator;

public:
    class RowView {
    public:
        explicit RowView(Matrix& matrix, size_t row_index) : matrix_(matrix), row_index_(row_index) {
        }

        T& operator[](size_t index) {
            std::pair<size_t, size_t> key{row_index_, index};
            const auto [iter, inserted] = matrix_.values_.emplace(key, DefaultValue);
            std::ignore = inserted;
            return iter->second;
        }

    private:
        Matrix& matrix_;
        const size_t row_index_;
    };

    class ConstRowView {
    public:
        explicit ConstRowView(const Matrix& matrix, size_t row_index) : matrix_(matrix), row_index_(row_index) {
        }

        T operator[](size_t index) const {
            std::pair<size_t, size_t> key{row_index_, index};
            const auto iter = matrix_.values_.find(key);
            if (iter == matrix_.values_.end()) {
                return DefaultValue;
            }
            return iter->second;
        }

    private:
        const Matrix& matrix_;
        const size_t row_index_;
    };

    class Iterator {
    public:
        Iterator(const Matrix& matrix, ValuesMapIterator map_iterator) : matrix_(matrix), map_iterator_(map_iterator) {
            SkipDefaultValues();
        }

        Iterator& operator++() {
            ++map_iterator_;
            SkipDefaultValues();
            return *this;
        }

        bool operator==(const Iterator& other) {
            return map_iterator_ == other.map_iterator_;
        }

        bool operator!=(const Iterator& other) {
            return !(*this == other);
        }

        std::tuple<size_t, size_t, T> operator*() {
            assert(map_iterator_ != matrix_.values_.end());
            assert(map_iterator_->second != DefaultValue);
            return {map_iterator_->first.first, map_iterator_->first.second, map_iterator_->second};
        }

    private:
        void SkipDefaultValues() {
            while (map_iterator_ != matrix_.values_.end() && map_iterator_->second == DefaultValue) {
                ++map_iterator_;
            }
        }

        const Matrix& matrix_;
        ValuesMapIterator map_iterator_;
    };

    [[nodiscard]] size_t size() {
        Canonize();
        return values_.size();
    }

    RowView operator[](size_t index) {
        return RowView(*this, index);
    }

    ConstRowView operator[](size_t index) const {
        return ConstRowView(*this, index);
    }

    Iterator begin() const {
        return Iterator(*this, values_.begin());
    }

    Iterator end() const {
        return Iterator(*this, values_.end());
    }

private:
    void Canonize() {
        auto iter = values_.begin();
        while (iter != values_.end()) {
            if (iter->second == DefaultValue) {
                iter = values_.erase(iter);
            } else {
                ++iter;
            }
        }
    }

    ValuesMap values_;
};
