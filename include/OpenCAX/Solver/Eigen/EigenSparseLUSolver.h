#pragma once

#include <OpenCAX/Solver/LinearSolver.h>


namespace OpenCAX
{


class EigenSparseLUSolver :
    public LinearSolver
{

public:

    const char* name() const override
    {
        return "Eigen::SparseLU";
    }


    SolverResult solve(
        LinearSystem&,
        const SolverOptions&
    ) override;


};


}