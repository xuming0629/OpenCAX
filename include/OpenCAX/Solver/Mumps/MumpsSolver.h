#pragma once
#include <OpenCAX/Solver/LinearSolver.h>
namespace OpenCAX {
class MumpsSolver:public LinearSolver{
public:
const char* name()const override{return "MUMPS";}
SolverResult solve(LinearSystem&,const SolverOptions&)override;
};
}
