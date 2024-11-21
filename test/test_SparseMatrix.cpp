#include "SparseMatrix.h"
#include "gtest.h"

// CMatrix
TEST(CMatrix, can_create_cmatrix)
{
    std::size_t n = 5;
    std::size_t nz = 8;

    int* value = new int[nz] {7, 14, 21, 28, 35, 42, 49, 56};
    int* row = new int[nz] {0, 0, 1, 2, 3, 3, 4, 4};
    int* col = new int[nz] {1, 3, 2, 1, 0, 2, 1, 3};

    ASSERT_NO_THROW(CMatrix<int>A(n, nz, value, row, col));
}

TEST(CMatrix, cant_create_cmatrix_with_wrong_size)
{
    std::size_t n = 0;
    std::size_t nz = 9;

    int* value = new int[nz] {1, 2, 3, 4, 8, 5, 7, 1, 6};
    int* row = new int[nz] {0, 0, 1, 1, 3, 3, 4, 4, 4};
    int* col = new int[nz] {0, 4, 2, 3, 3, 5, 2, 3, 5};

    ASSERT_ANY_THROW(CMatrix<int>A(n, nz, value, row, col));
}

TEST(CMatrix, can_do_Ax_is_equal_to_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;

    int* value = new int[nz] {1, 2, 3, 4, 8, 5, 7, 1, 6};
    int* row = new int[nz] {0, 0, 1, 1, 3, 3, 4, 4, 4};
    int* col = new int[nz] {0, 4, 2, 3, 3, 5, 2, 3, 5};

    CMatrix<int>A(n, nz, value, row, col);

    int* x = new int[n] {8, 5, 0, 4, 3, 1};
    int* xres = new int[n] {14, 16, 0, 37, 10, 0};
    int* _x = new int[n];

    _x = A * x;

    int flag = 1;

    for (std::size_t i = 0; i < n; i++)
    {
        if (_x[i] != xres[i])
        {
            flag = 0;
        }
    }

    EXPECT_EQ(1, flag);
}

TEST(CMatrix, can_do_fast_Ax_is_equal_to_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;

    int* value = new int[nz] {1, 2, 3, 4, 8, 5, 7, 1, 6};
    int* row = new int[nz] {0, 0, 1, 1, 3, 3, 4, 4, 4};
    int* col = new int[nz] {0, 4, 2, 3, 3, 5, 2, 3, 5};

    CMatrix<int>A(n, nz, value, row, col);

    int* x = new int[n] {8, 5, 0, 4, 3, 1};
    int* xres = new int[n] {14, 16, 0, 37, 10, 0};
    int* _x = new int[n];

    _x = A.fstMul(x);

    int flag = 1;

    for (std::size_t i = 0; i < n; i++)
    {
        if (_x[i] != xres[i])
        {
            flag = 0;
        }
    }

    EXPECT_EQ(1, flag);
}

TEST(CMatrix, can_a_is_equal_b)
{
    std::size_t n = 4;
    std::size_t nz = 6;

    int* value = new int[nz] {10, 20, 30, 40, 50, 60};
    int* value1 = new int[nz] {5, 15, 25, 35, 45, 55};
    int* row = new int[nz] {0, 0, 1, 1, 2, 3};
    int* col = new int[nz] {0, 1, 1, 2, 2, 3};

    CMatrix<int> A(n, nz, value, row, col);
    CMatrix<int> B(n, nz, value1, row, col);

    A = B;

    EXPECT_EQ(B, A);
}

TEST (CMatrix, can_a_is_not_equal_b)
{
    std::size_t n = 5;
    std::size_t nz = 7;

    int* value = new int[nz] {10, 20, 30, 40, 50, 60, 70};
    int* value1 = new int[nz] {15, 25, 35, 45, 55, 65, 75};
    int* row = new int[nz] {0, 0, 1, 2, 2, 3, 4};
    int* col = new int[nz] {0, 1, 1, 2, 3, 3, 4};

    CMatrix<int> A(n, nz, value, row, col);
    CMatrix<int> B(n, nz, value1, row, col);

    EXPECT_NE(B, A);
}

// CRSMatrix
TEST(CRSMatrix, can_create_crsmatrix)
{
    std::size_t n = 5;
    std::size_t nz = 8;

    int* value = new int[nz] {7, 14, 21, 28, 35, 42, 49, 56};
    int* row_index = new int[n + 1] {0, 2, 3, 4, 6, 8};
    int* col = new int[nz] {1, 3, 2, 1, 0, 2, 1, 3};

    ASSERT_NO_THROW(CRSMatrix<int> A(n, nz, value, col, row_index));

    delete[] value;
    delete[] row_index;
    delete[] col;
}

TEST(CRSMatrix, cant_create_crsmatrix_with_wrong_size)
{
    std::size_t n = 0;
    std::size_t nz = 9;

    int* value = new int[nz] {1, 2, 3, 4, 8, 5, 7, 1, 6};
    int* row_index = new int[n+1] {0};
    int* col = new int[nz] {0, 4, 2, 3, 3, 5, 2, 3, 5};

    ASSERT_ANY_THROW(CRSMatrix<int>A(n, nz, value, col, row_index));
}

TEST(CRSMatrix, can_do_Ax_is_equal_to_b)
{
    std::size_t n = 6;
    std::size_t nz = 9;

    int* value = new int[nz] {1, 2, 3, 4, 8, 5, 7, 1, 6};
    int* row_index = new int[n+1] {0, 2, 4, 4, 6, 9, 9 };
    int* col = new int[nz] {0, 4, 2, 3, 3, 5, 2, 3, 5};

    CRSMatrix<int>A(n, nz, value, col, row_index);

    int* x = new int[n] {8, 5, 0, 4, 3, 1};
    int* xres = new int[n] {14, 16, 0, 37, 10, 0};
    int* _x = new int[n];

    _x = A * x;

    int flag = 1;

    for (std::size_t i = 0; i < n; i++)
    {
        if (_x[i] != xres[i])
        {
            flag = 0;
        }
    }

    EXPECT_EQ(1, flag);
}

TEST (CRSMatrix, can_a_is_equal_b)
{
    std::size_t n = 4;
    std::size_t nz = 6;

    int* value = new int[nz] {10, 20, 30, 40, 50, 60};
    int* value1 = new int[nz] {5, 15, 25, 35, 45, 55};
    int* row = new int[nz] {0, 0, 1, 1, 2, 3};
    int* col = new int[nz] {0, 1, 1, 2, 2, 3};

    CMatrix<int> A(n, nz, value, row, col);
    CMatrix<int> B(n, nz, value1, row, col);

    A = B;

    EXPECT_EQ(B, A);
}

TEST (CRSMatrix, can_a_is_not_equal_b)
{
    std::size_t n = 5;
    std::size_t nz = 7;

    int* value = new int[nz] {10, 20, 30, 40, 50, 60, 70};
    int* value1 = new int[nz] {15, 25, 35, 45, 55, 65, 75};
    int* row = new int[nz] {0, 0, 1, 2, 2, 3, 4};
    int* col = new int[nz] {0, 1, 1, 2, 3, 3, 4};

    CMatrix<int> A(n, nz, value, row, col);
    CMatrix<int> B(n, nz, value1, row, col);

    EXPECT_NE(B, A);
}