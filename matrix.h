#pragma once
#include <map>
#include <algorithm>
#include <numeric>
#include <functional>
#include <cassert>

template <class T, T DefaultValue>
class Matrix {
private:
    using ValuesMap = std::map<size_t, T>;
    using ValuesMapIterator = typename ValuesMap::const_iterator;

public:
    class Row {
    public:
        T& operator [](size_t index) {
            auto iter = values_.find(index);
            if (iter == values_.end()) {
                bool inserted = false;
                std::tie(iter, inserted) = values_.insert({index, DefaultValue});
                assert(inserted);
            }
            return iter->second;
        }

        T operator [](size_t index) const {
            auto iter = values_.find(index);
            if (iter == values_.end()) {
                return DefaultValue;
            }
            return *iter;
        }

        [[nodiscard]] size_t size() const {
            return std::count_if(values_.begin(), values_.end(), [](const auto& kv) {
                return kv.second != DefaultValue;
            });
        }

        class Iterator {
        public:
            Iterator(ValuesMapIterator iterator, ValuesMapIterator end_iterator)
                    : iterator_(std::move(iterator)), end_iterator_(std::move(end_iterator)) {
                SkipDefaultValues();
            }

            Iterator& operator++() {
                assert(iterator_ != end_iterator_);
                ++iterator_;
                SkipDefaultValues();
                return *this;
            }

            std::pair<size_t, T> operator*() const {
                assert(iterator_ != end_iterator_);
                return *iterator_;
            }

            bool operator==(const Iterator& other) const {
                assert(end_iterator_ == other.end_iterator_);
                return iterator_ == other.iterator_;
            }

            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

        private:
            void SkipDefaultValues() {
                while (iterator_ != end_iterator_ && iterator_->second == DefaultValue) {
                    ++iterator_;
                }
            }

            ValuesMapIterator iterator_;
            ValuesMapIterator end_iterator_;
        };

        Iterator begin() const {
            return Iterator{values_.begin(), values_.end()};
        }

        Iterator end() const {
            return Iterator{values_.end(), values_.end()};
        }

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

    private:
        std::map<size_t, T> values_;
    };

private:
    using RowsMap = std::map<size_t, Row>;
    using RowsMapIterator = typename RowsMap::const_iterator;
    using RowIterator = typename Row::Iterator;

public:
    class Iterator {
    public:
        Iterator(
                RowsMapIterator rows_map_iterator, RowsMapIterator end_rows_map_iterator)
                : rows_map_iterator_(std::move(rows_map_iterator))
                , end_rows_map_iterator_(std::move(end_rows_map_iterator))
                , row_iterator_((rows_map_iterator_ != end_rows_map_iterator_)
                        ? std::make_optional(rows_map_iterator->second.begin()) : std::nullopt) {
            SkipEmptyRows();
        }

        Iterator& operator++() {
            assert(rows_map_iterator_ != end_rows_map_iterator_);
            assert(row_iterator_ && *row_iterator_ != rows_map_iterator_->second.end());
            ++(*row_iterator_);
            SkipEmptyRows();
            return *this;
        }

        std::tuple<size_t, size_t, T> operator*() const {
            assert(rows_map_iterator_ != end_rows_map_iterator_);
            assert(row_iterator_ && *row_iterator_ != rows_map_iterator_->second.end());
            const auto& [second_index, value] = **row_iterator_;
            return {rows_map_iterator_->first, second_index, value};
        }

        bool operator==(const Iterator& other) const {
            assert(end_rows_map_iterator_ == other.end_rows_map_iterator_);
            return rows_map_iterator_ == other.rows_map_iterator_ && row_iterator_ == other.row_iterator_;
        }

        bool operator!=(const Iterator& other) const {
            return !(*this == other);
        }

    private:
        void SkipEmptyRows() {
            if (!row_iterator_) {
                return;
            }
            assert(rows_map_iterator_ != end_rows_map_iterator_);
            while (rows_map_iterator_ != end_rows_map_iterator_ && *row_iterator_ == rows_map_iterator_->second.end()) {
                ++rows_map_iterator_;
                row_iterator_ = (rows_map_iterator_ != end_rows_map_iterator_)
                        ? std::make_optional(rows_map_iterator_->second.begin()) : std::nullopt;
            }
            assert(!row_iterator_ ||
                    (rows_map_iterator_ != end_rows_map_iterator_ &&
                    *row_iterator_ != rows_map_iterator_->second.end()));
        }

        RowsMapIterator rows_map_iterator_;
        RowsMapIterator end_rows_map_iterator_;
        std::optional<RowIterator> row_iterator_;
    };

    Row& operator[](size_t index) {
        return rows_[index];
    }

    Row operator[](size_t index) const {
        const auto iter = rows_.find(index);
        if (iter == rows_.end()) {
            return Row{};
        }
        return iter->second;
    }

    [[nodiscard]] size_t size() const {
        return std::accumulate(rows_.begin(), rows_.end(), 0ul, [](size_t value, const auto& kv) {
            return value + kv.second.size();
        });
    }

    [[nodiscard]] size_t size() {
        Canonize();
        return static_cast<const Matrix>(*this).size();
    }

    Iterator begin() const {
        return Iterator(rows_.begin(), rows_.end());
    }

    Iterator end() const {
        return Iterator(rows_.end(), rows_.end());
    }



private:
    void Canonize() {
        for (auto& [index, row] : rows_) {
            std::ignore = index;
            row.Canonize();
        }
    }

    RowsMap rows_;
};
