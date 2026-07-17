#pragma once
#include <OpenCAX/Solver/LinearSolver.h>
namespace OpenCAX {
class PetscSolver:public LinearSolver{
public:
const char* name()const override{return "PETSc";}
SolverResult solve(LinearSystem&,const SolverOptions&)override;
};
}
