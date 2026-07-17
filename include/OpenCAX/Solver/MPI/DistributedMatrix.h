#pragma once
#include <OpenCAX/Solver/SparseMatrix.h>

namespace OpenCAX {

class DistributedMatrix
{
public:

    SparseMatrix local;

    int global_rows=0;
    int global_cols=0;

    int row_start=0;
    int row_end=0;
};

}
