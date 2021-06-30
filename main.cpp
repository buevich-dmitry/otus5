#include <iostream>
#include "matrix.h"
#include <cassert>

template <class T, T DefaultValue>
void PrintMatrixFragment(
        const Matrix<T, DefaultValue>& matrix, size_t i_begin, size_t i_end, size_t j_begin, size_t j_end) {
    assert(i_begin < i_end && j_begin < j_end);
    for (size_t i = i_begin; i < i_end; ++i) {
        for (size_t j = j_begin; j < j_end; ++j) {
            std::cout << ((j != j_begin) ? " " : "") << matrix[i][j];
        }
        std::cout << '\n';
    }
}

template <class T, T DefaultValue>
void PrintAllFilledCells(const Matrix<T, DefaultValue>& matrix) {
    for (const auto& [i, j, value] : matrix) {
        std::cout << '(' << i << ',' << j << "): " << value << '\n';
    }
}

int main() {
    Matrix<int, 0> matrix;
    for (size_t i = 0; i < 10; ++i) {
        matrix[i][i] = i;
        matrix[i][9 - i] = 9 - i;
    }
    std::cout << "Matrix fragment:\n";
    PrintMatrixFragment(matrix, 1, 9, 1, 9);
    std::cout << "\nMatrix size: " << matrix.size() << '\n';
    std::cout << "All filled cells:\n";
    PrintAllFilledCells(matrix);
    return 0;
}