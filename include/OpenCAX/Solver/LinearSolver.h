#pragma once

#include <OpenCAX/Solver/LinearSystem.h>
#include <OpenCAX/Solver/SolverOptions.h>
#include <OpenCAX/Solver/SolverResult.h>

namespace OpenCAX {

class LinearSolver
{
public:

    virtual ~LinearSolver()=default;

    virtual const char* name() const=0;

    virtual SolverResult solve(
        LinearSystem&,
        const SolverOptions&)=0;

    virtual void clear(){}

};

}
