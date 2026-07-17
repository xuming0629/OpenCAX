#pragma once
#include <memory>
#include <OpenCAX/Solver/LinearSolver.h>

namespace OpenCAX {

class SolverFactory
{
public:

static std::unique_ptr<LinearSolver>
create(const SolverOptions&);

};

}
