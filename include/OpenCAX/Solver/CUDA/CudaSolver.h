#pragma once
#include <OpenCAX/Solver/LinearSolver.h>
namespace OpenCAX {
class CudaSolver:public LinearSolver{
public:
const char* name()const override{return "CUDA";}
SolverResult solve(LinearSystem&,const SolverOptions&)override;
};
}
