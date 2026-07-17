#pragma once
#include <OpenCAX/Solver/LinearSolver.h>
namespace OpenCAX {
class EigenBiCGSTABSolver:public LinearSolver{
public:
const char* name()const override{return "Eigen::BiCGSTAB";}
SolverResult solve(LinearSystem&,const SolverOptions&)override;
};
}
