// @uw.edu
// AMATH 483-583
// strassen.cpp : starter code for Strassen implementation

#include <iostream>
#include <vector>

using namespace std;

template <typename T>
vector<vector<T>> addMatrix(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    vector<vector<T>> C(n, vector<T>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

template <typename T>
vector<vector<T>> subtractMatrix(const vector<vector<T>> &A, const vector<vector<T>> &B)
{
    int n = A.size();
    int m = A[0].size();
    vector<vector<T>> C(n, vector<T>(m));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

template <typename T>
void printMatrix(const vector<vector<T>> &matrix)
{
    int n = matrix.size();
    int m = matrix[0].size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cout << matrix[i][j] << " ";
        }
        cout << endl;
    }
}

template <typename T>
vector<vector<T>> strassenMultiply(const vector<vector<T>> &A, const vector<vector<T>> &B)
{

    // Check if A and B are square matrices 
    if (A.size() != A[0].size() || B.size() != B[0].size())
    {
        throw invalid_argument("Matrices must be square for Strassen's algorithm.");
    }

    // Check A and B have compatible dimensions
    if (A[0].size() != B.size())
    {
        throw invalid_argument("Matrices must have compatible dimensions for multiplication.");
    }

     // Now recursively divide the matrices into quadrants
     int n = A.size();
     int k = n / 2; // Size of the submatrices

    // Verify N is a power of 2
    // Do the ole bitwise & trick
    if ((n & (n - 1)) != 0)
    {
        throw invalid_argument("Matrices must be of size 2^n for Strassen's algorithm.");
    }

    // Check base case
    if (A.size() == 1 && A[0].size() == 1 && B.size() == 1 && B[0].size() == 1)
    {
        return {{A[0][0] * B[0][0]}};
    }

    // A = {{A11,A12}, {A21,A22}} B = {{B11,B12}, {B21,B22}}
    // Recursively fill the submatrices
    vector<vector<T>> A11(k, vector<T>(k)), A12(k, vector<T>(k)), A21(k, vector<T>(k)), A22(k, vector<T>(k));
    vector<vector<T>> B11(k, vector<T>(k)), B12(k, vector<T>(k)), B21(k, vector<T>(k)), B22(k, vector<T>(k));

    for (int i = 0; i < k; i++)
    {
        std::memcpy(A11[i].data(), A[i].data(), k * sizeof(T));
        std::memcpy(A12[i].data(), A[i].data() + k, k * sizeof(T));
        std::memcpy(A21[i].data(), A[i + k].data(), k * sizeof(T));
        std::memcpy(A22[i].data(), A[i + k].data() + k, k * sizeof(T));
        std::memcpy(B11[i].data(), B[i].data(), k * sizeof(T));
        std::memcpy(B12[i].data(), B[i].data() + k, k * sizeof(T));
        std::memcpy(B21[i].data(), B[i + k].data(), k * sizeof(T));
        std::memcpy(B22[i].data(), B[i + k].data() + k, k * sizeof(T));
    }

    auto M1 = strassenMultiply(addMatrix(A11, A22), addMatrix(B11, B22));
    auto M2 = strassenMultiply(addMatrix(A21, A22), B11);
    auto M3 = strassenMultiply(A11, subtractMatrix(B12, B22));
    auto M4 = strassenMultiply(A22, subtractMatrix(B21, B11));
    auto M5 = strassenMultiply(addMatrix(A11, A12), B22);
    auto M6 = strassenMultiply(subtractMatrix(A21, A11), addMatrix(B11, B12));
    auto M7 = strassenMultiply(subtractMatrix(A12, A22), addMatrix(B21, B22));

    std::vector<std::vector<T>> C(n, std::vector<T>(n));
    std::vector<std::vector<T>> C11 = addMatrix(subtractMatrix(addMatrix(M1, M4), M5), M7);
    std::vector<std::vector<T>> C12 = addMatrix(M3, M5);
    std::vector<std::vector<T>> C21 = addMatrix(M2, M4);
    std::vector<std::vector<T>> C22 = addMatrix(addMatrix(subtractMatrix(M1, M2), M3), M6);


    for (int i = 0; i < k; i++)
    {
        // Top Half
        std::memcpy(C[i].data(), C11[i].data(), k * sizeof(T));
        std::memcpy(C[i].data() + k, C12[i].data(), k * sizeof(T));
        
        // Lower Half
        std::memcpy(C[i+k].data(), C21[i].data(), k * sizeof(T));
        std::memcpy(C[i+k].data() + k, C22[i].data(), k * sizeof(T));
    }
    
    return C;
}

// int
template vector<vector<int>> addMatrix<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template vector<vector<int>> subtractMatrix<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template vector<vector<int>> strassenMultiply<int>(const vector<vector<int>> &A, const vector<vector<int>> &B);
template void printMatrix<int>(const vector<vector<int>> &matrix);
// double
template vector<vector<double>> addMatrix<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template vector<vector<double>> subtractMatrix<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template vector<vector<double>> strassenMultiply<double>(const vector<vector<double>> &A, const vector<vector<double>> &B);
template void printMatrix<double>(const vector<vector<double>> &matrix);
