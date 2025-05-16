#ifndef FILE_SWAPS_HPP
#define FILE_SWAPS_HPP

#include <fstream>

void swapRowsInFile(std::fstream &file, int nRows, int nCols, int i, int j) {

    if (i < 0 || i >= nRows || j < 0 || j >= nRows) {
        throw std::out_of_range("Row index out of range");
    }
    if (i == j) {
        return; // No need to swap the same row
    }

    // File has matrix as single vector, in column major order
    // From before, we know that
    // Laid flat in nrow*ncol*size_of(double) bytes in file
    // Swap rows i and j
    // In column mahjor order, to get to m,nth element, we need to go to
    // (col * nrow + row) pos * size_of(double) bytes to read from flatfile...
    const size_t elementSize = sizeof(double);

    // In column-major order, rows are interleaved across columns.
    // To swap rows i and j, iterate over each column and swap the elements at row i and row j.
    for (int col = 0; col < nCols; col++) {
        size_t posI = (col * nRows + i) * elementSize;
        size_t posJ = (col * nRows + j) * elementSize;

        double temp_i, temp_j;

        // Read the element at row i, column col
        file.seekg(posI, std::ios::beg);
        if (!file.read(reinterpret_cast<char*>(&temp_i), elementSize)) {
            throw std::ios_base::failure("Failed to read value at row i");
        }

        // Read the element at row j, column col
        file.seekg(posJ, std::ios::beg);
        if (!file.read(reinterpret_cast<char*>(&temp_j), elementSize)) {
            throw std::ios_base::failure("Failed to read value at row j");
        }

        // Write the element from row j into row i's position
        file.seekp(posI, std::ios::beg);
        if (!file.write(reinterpret_cast<char*>(&temp_j), elementSize)) {
            throw std::ios_base::failure("Failed to write value at row i");
        }

        // Write the element from row i into row j's position
        file.seekp(posJ, std::ios::beg);
        if (!file.write(reinterpret_cast<char*>(&temp_i), elementSize)) {
            throw std::ios_base::failure("Failed to write value at row j");
        }
    }

    // Flush to make sure its written
    file.flush();
}

void swapColsInFile(std::fstream &file, int nRows, int nCols, int i, int j) {
    if (i < 0 || i >= nCols || j < 0 || j >= nCols) {
        throw std::out_of_range("Col index out of range");
    }
    if (i == j) {
        return; // No need to swap the same row
    }

    const size_t elementSize = sizeof(double);
    const size_t colSize = nRows * elementSize;

    // Even easier now, we can read the entire column at once and swap
    size_t colI = (i * nRows) * elementSize;
    size_t colJ = (j * nRows) * elementSize;

    double* temp_col_i = new double[nRows];
    double* temp_col_j = new double[nRows];

    // Read Column i
    file.seekg(colI, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(temp_col_i), colSize)) {
        throw std::ios_base::failure("Failed to read column i");
    }

    // Read Column j
    file.seekg(colJ, std::ios::beg);
    if (!file.read(reinterpret_cast<char*>(temp_col_j), colSize)) {
        throw std::ios_base::failure("Failed to read column j");
    }

    // Write column j into column i's position
    file.seekp(colI, std::ios::beg);
    if (!file.write(reinterpret_cast<char*>(temp_col_j), colSize)) {
        throw std::ios_base::failure("Failed to write to column i");
    }

    // Write the element from row i into row j's position
    file.seekp(colJ, std::ios::beg);
    if (!file.write(reinterpret_cast<char*>(temp_col_i), colSize)) {
        throw std::ios_base::failure("Failed to write to column j");
    }

    // Flush to make sure its written
    file.flush();


    // Clean up memory
    delete[] temp_col_i;
    delete[] temp_col_j;
}

#endif