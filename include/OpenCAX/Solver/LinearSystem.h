#pragma once

#include <OpenCAX/Solver/SparseMatrix.h>
#include <OpenCAX/Solver/Vector.h>

namespace OpenCAX {

class LinearSystem
{
public:

    SparseMatrix A;

    Vector b;

    Vector x;


    bool valid() const
    {
        return
            A.rows()==A.cols()
            &&
            A.rows()==b.size()
            &&
            x.size()==b.size();
    }
};

}
