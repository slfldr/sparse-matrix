#include "SparseMatrix.h"
#include <iostream>

int main()
{
    //CMatrix
    std::size_t n = 3;
    std::size_t nz = 4;

    double values[] = {10, 20, 30, 40};
    int rows[] = {0, 1, 2, 1};
    int cols[] = {0, 1, 2, 0};

    double* pValues = values;
    int* pRows = rows;
    int* pCols = cols;

    CMatrix<double> matrix(n, nz, pValues, pRows, pCols);

    double x[] = {1, 2, 3};
    double* result = matrix * x;

    std::cout << "Result of CMatrix multiplication:" << std::endl;
    for (std::size_t i = 0; i < n; ++i)
    {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    delete[] result;

    //CRSMatrix
    int rowIndex[] = {0, 1, 3, 4};

    int* pRowIndex = rowIndex;

    CRSMatrix<double> crsMatrix(n, nz, pValues, pCols, pRowIndex);

    result = crsMatrix * x;

    std::cout << "Result of CRSMatrix multiplication:" << std::endl;
    for (std::size_t i = 0; i < n; ++i)
    {
        std::cout << result[i] << " ";
    }
    std::cout << std::endl;

    delete[] result;

    return 0;
}