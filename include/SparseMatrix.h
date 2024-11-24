#ifndef SparseMatrix_h

#define SparseMatrix_h

#include <iostream>

// Coordinate storage format
template<typename T> class CMatrix 
{ 
private:
    std::size_t n;
    std::size_t nz;

    T* value;

    int* row;
    int* col;
public:
    CMatrix(std::size_t _n, std::size_t _nz, T* (&_value), int* (&_row), int* (&_col))
    {
        if (_n == 0 || _nz == 0)
        {
            throw std::out_of_range("Incorrect size");
        }
        else
        {
            value = new T[_nz];

            if (value == nullptr)
            {
                throw std::bad_alloc();
            }
            else
            {
                std::copy(_value, _value + _nz, value);
            }

            row = new int[_nz];

            if (row == nullptr)
            {
                throw std::bad_alloc();
            }
            else
            {
                std::copy(_row, _row + _nz, row);
            }

            col = new int[_nz];

            if (col == nullptr)
            {
                throw std::bad_alloc();
            }
            else
            {
                std::copy(_col, _col + _nz, col);
            }

            n = _n;
            nz = _nz;
        }
    }
    CMatrix(const CMatrix& m)
    {
        value = new T[m.nz];

        if (value == nullptr)
        {
            throw std::bad_alloc();
        }
        else
        {
            std::copy(m.value, m.value + m.nz, value);
        }

        row = new int[m.nz];

        if (row == nullptr)
        {
            throw std::bad_alloc();
        }
        else
        {
            std::copy(m.row, m.row + m.nz, row);
        }

        col = new int[m.nz];

        if (col == nullptr)
        {
            throw std::bad_alloc();
        }
        else
        {
            std::copy(m.col, m.col + m.nz, col);
        }

        n = m.n;
        nz = m.nz;
    }
    CMatrix(CMatrix&& m) noexcept 
    {
        value = nullptr;
        row = nullptr;
        col = nullptr;

        swap(*this, m);
    }
    ~CMatrix()
    {
        delete[] value;
        delete[] row;
        delete[] col;

        value = nullptr;
        row = nullptr;
        col = nullptr;
    }

    CMatrix& operator=(const CMatrix& m)
    {
        if (&m == this)
        {
            return *this;
        }

        if(nz!=m.nz)
        {
            T* tmpv=new T[m.nz];

            int* tmpr = new int[m.nz];
            int* tmpc = new int[m.nz];

            delete[] value;
            delete[] row;
            delete[] col;

            value = tmpv;
            row = tmpr;
            col = tmpc;
        }
        else
        {
            std::copy(m.value, m.value+m.nz, value);
            std::copy(m.row, m.row+m.nz, row);
            std::copy(m.col, m.col+m.nz, col);

            n = m.n;
            nz = m.nz;
        }

        return *this;
    }

    CMatrix& operator=(CMatrix&& m) noexcept
    {
        swap(*this, m);

        return *this;
    }

    friend void swap(CMatrix& lhs, CMatrix& rhs) noexcept
    {
        std::swap(lhs.n, rhs.n);
        std::swap(lhs.nz, rhs.nz);
        std::swap(lhs.value, rhs.value);
        std::swap(lhs.row, rhs.row);
        std::swap(lhs.col, rhs.col);
    }

    T operator*(const T* x)
    {
        T* b = new T[n]{0};

        for (std::size_t i = 0; i < nz; i++)
        {
            b[row[i]] += value[i] * x[col[i]];
        }

        return b;
    }
    T fstMul(const T* x)
    {
        T* b = new T[n]{0};

        std::size_t j = 0;

        for(std::size_t i = 0; i < n; i++)
        { 
            while(j<nz && row[j] == i)
            {
                b[i] += value[j] * x[col[j]];
                j++;
            }
        }

        return b;
    }

    bool operator==(const CMatrix& m) const noexcept
    {
        if (n != m.n || nz != m.nz)
        {
            return 0;
        }

        for (std::size_t i = 0; i < nz; i++)
        {

            if (value[i] != m.value[i] || row[i] != m.row[i] || col[i] != m.col[i])
            {
                return 0;
            }
        }

        return 1;
    }
    bool operator!=(const CMatrix& m) const noexcept
    {
        return !((*this) == m);
    }
};

// Compressed string format for matrix storage
template<typename T> class CRSMatrix
{
private:
    std::size_t n;
    std::size_t nz;

    T* value;

    int* col;
    int* row_index;
public:
    CRSMatrix(std::size_t _n, std::size_t _nz, T* (&_value), int* (&_col), int* (&_row_index))
    {
        if (_n == 0 || _nz == 0)
        {
            throw std::out_of_range("Incorrect size");
        }
        else
        {
            value=new T[_nz];

            if (value == nullptr)
            {
                throw std::bad_alloc();
            }
            else
            {
                std::copy(_value, _value + _nz, value);
            }

            col = new int[_nz];

            if (col == nullptr)
            {
                throw std::bad_alloc();
            }
            else
            {
                std::copy(_col, _col + _nz, col);
            }

            row_index = new int[_n+1];

            if (row_index == nullptr)
            {
                throw std::bad_alloc();
            }
            else
            {
                std::copy(_row_index, _row_index + _n + 1, row_index);
            }

            n = _n;
            nz = _nz;
        }
    }
    CRSMatrix(const CRSMatrix& m)
    {
        value = new T[m.nz];

        if (value == nullptr)
        {
            throw std::bad_alloc();
        }
        else
        {
            std::copy(m.value, m.value + m.nz, value);
        }

        col = new int[m.nz];

        if (col == nullptr)
        {
            throw std::bad_alloc();
        }
        else
        {
            std::copy(m.col, m.col + m.nz, col);
        }

        row_index = new int[m.n+1];

        if (row_index == nullptr)
        {
            throw std::bad_alloc();
        }
        else
        {
            std::copy(m.row_index, m.row_index + m.n + 1, row_index);
        }

        n = m.n;
        nz = m.nz;
    }
    CRSMatrix(CRSMatrix&& m) noexcept
    {
        value = nullptr;
        row_index = nullptr;
        col = nullptr;
        swap(*this, m);
    }
    ~CRSMatrix()
    {
        delete[] value;
        delete[] row_index;
        delete[] col;

        value = nullptr;
        row_index = nullptr;
        col = nullptr;
    }

    CRSMatrix& operator=(const CRSMatrix& m)
    {
        if (&m == this)
        {
            return *this;
        }

        if(nz!=m.nz)
        {
            T* tmpv = new T[m.nz];
            int* tmpr = new int[m.n+1];
            int* tmpc = new int[m.nz];

            delete[] value;
            delete[] row_index;
            delete[] col;

            value = tmpv;
            row_index = tmpr;
            col = tmpc;
        }
        else
        {
            std::copy(m.value, m.value+m.nz, value);
            std::copy(m.row_index, m.row_index+m.n+1, row_index);
            std::copy(m.col, m.col + m.nz,col);

            n=m.n;
            nz=m.nz;
        }

        return *this;
    }
    CRSMatrix& operator=(CRSMatrix&& m) noexcept
    {
        swap(*this, m);

        return *this;
    }

    friend void swap(CRSMatrix& lhs, CRSMatrix& rhs) noexcept
    {
        std::swap(lhs.n, rhs.n);
        std::swap(lhs.nz, rhs.nz);
        std::swap(lhs.value, rhs.value);
        std::swap(lhs.row_index, rhs.row_index);
        std::swap(lhs.col, rhs.col);
    }

    T operator*(const T* x)
    {
        T* b = new T[n]{};

        for(std::size_t i = 0; i < n; i++)
        {
            for(std::size_t j = row_index[i]; j < row_index[i+1]; j++)
            {
                b[i] += value[j] * x[col[j]];
            } 
        }

        return b;
    }

    bool operator==(const CRSMatrix& m) const noexcept
    {
        if (n != m.n || nz != m.nz)
        {
            return 0;
        }

        for (std::size_t i = 0; i < nz; i++)
        {
            if (value[i] != m.value[i] || col[i] != m.col[i])
            {
                return 0;
            }
        }

        for (std::size_t i = 0; i < n + 1; i++)
        {
            if (row_index[i] != m.row_index[i])
            {
                return 0;
            }
        }

        return 1;
    }
    bool operator!=(const CRSMatrix& m) const noexcept
    {
        return !((*this) == m);
    }
};

#endif
