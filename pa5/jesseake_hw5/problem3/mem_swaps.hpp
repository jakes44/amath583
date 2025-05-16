#ifndef MEM_SWAPS_HPP
#define MEM_SWAPS_HPP

#include <vector>
#include <cstring>

void swapRows(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {

    if (i < 0 || i >= nRows || j < 0 || j >= nRows) {
        throw std::out_of_range("Row index out of range");
    }
    if (i == j) {
        return; // No need to swap the same row
    }

    // matrix as single vector, in column major order
    // From before, we know that
    // Laid flat in nrow*ncol*size_of(double) bytes in file
    // Swap rows i and j
    // In column mahjor order, to get to m,nth element, we need to go to
    // (col * nrow + row) pos * size_of(double) bytes to read from flatfile...

    // In column-major order, rows are interleaved across columns.
    // To swap rows i and j, iterate over each column and swap the elements at row i and row j.
    for (int col = 0; col < nCols; col++) {
        size_t posI = (col * nRows + i);
        size_t posJ = (col * nRows + j);

        double temp_i, temp_j;

        temp_i = matrix[posI];
        temp_j = matrix[posJ];

        matrix[posI] = temp_j;
        matrix[posJ] = temp_i;
    }
}

void swapCols(std::vector<double> &matrix, int nRows, int nCols, int i, int j) {
    if (i < 0 || i >= nCols || j < 0 || j >= nCols) {
        throw std::out_of_range("Col index out of range");
    }
    if (i == j) {
        return; // No need to swap the same row
    }

    const size_t elementSize = sizeof(double);
    const size_t colSize = nRows * elementSize;

    // Even easier now, we can read the entire column at once and swap
    size_t colI = (i * nRows);
    size_t colJ = (j * nRows);

    double* temp_col_i = new double[nRows];
    double* temp_col_j = new double[nRows];

    // Read Column i
    std::memcpy(temp_col_i, (matrix.data() + colI), colSize);

    // Read Column j
    std::memcpy(temp_col_j, matrix.data() + colJ, colSize);

    // Write column j into column i's position
    std::memcpy(matrix.data() + colI, temp_col_j, colSize);

    // Write the element from row i into row j's position
    std::memcpy(matrix.data() + colJ, temp_col_i, colSize);

    // Clean up memory
    delete[] temp_col_i;
    delete[] temp_col_j;
}

#endif