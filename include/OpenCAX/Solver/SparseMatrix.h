#pragma once
#include <vector>

namespace OpenCAX {

class SparseMatrix
{
public:

    using Index=int;
    using Scalar=double;

    void resize(int r,int c)
    {
        rows_=r;
        cols_=c;
        row_ptr.resize(r+1);
    }

    int rows() const {return rows_;}
    int cols() const {return cols_;}

    std::vector<Index> row_ptr;
    std::vector<Index> col_indices;
    std::vector<Scalar> values;

private:

    int rows_=0;
    int cols_=0;
};

}
