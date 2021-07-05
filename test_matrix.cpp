#define BOOST_TEST_MODULE test_allocator

#include "matrix.h"
#include <set>

#include <boost/test/unit_test.hpp>

template <class T, T DefaultValue>
void CheckMatrix(
        const Matrix<T, DefaultValue>& matrix, const std::set<std::tuple<size_t, size_t, T>>& expected_values) {
    BOOST_CHECK(matrix.size() == expected_values.size());
    auto lhs_iter = matrix.begin();
    auto rhs_iter = expected_values.begin();
    while (lhs_iter != matrix.end() && rhs_iter != expected_values.end()) {
        BOOST_CHECK(*lhs_iter == *rhs_iter);
        ++lhs_iter;
        ++rhs_iter;
    }
    BOOST_CHECK(lhs_iter == matrix.end() && rhs_iter == expected_values.end());
    for (const auto& [i, j, value] : expected_values) {
        BOOST_CHECK(matrix[i][j] == value);
    }
}

BOOST_AUTO_TEST_SUITE(test_matrix)

BOOST_AUTO_TEST_CASE(test_1) {
    Matrix<int, -1> matrix;
    BOOST_CHECK(matrix.size() == 0);
    auto a = matrix[0][0];
    BOOST_CHECK(a == -1);
    BOOST_CHECK(matrix.size() == 0);
    matrix[100][100] = 314;
    BOOST_CHECK(matrix[100][100] == 314);
    BOOST_CHECK(matrix.size() == 1);
    CheckMatrix(matrix, {{100, 100, 314}});
}

BOOST_AUTO_TEST_CASE(test_2) {
    Matrix<int, 0> matrix;
    CheckMatrix(matrix, {});
    for (size_t i = 0; i < 6; ++i) {
        matrix[i][i] = i;
        matrix[i][5 - i] = 5 - i;
    }
    BOOST_CHECK_EQUAL(matrix.size(), 10);
    CheckMatrix(matrix, {{1, 1, 1}, {2, 2, 2}, {3, 3, 3}, {4, 4, 4}, {5, 5, 5},
                         {0, 5, 5}, {1, 4, 4}, {2, 3, 3}, {3, 2, 2}, {4, 1, 1}});
}

}
