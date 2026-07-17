#pragma once
#include <OpenCAX/Solver/SolverTypes.h>
#include <string>

namespace OpenCAX {

struct SolverResult
{
    SolverStatus status = SolverStatus::Failed;

    int iterations = 0;

    double residual_norm = 0.0;

    double relative_residual = 0.0;

    double solve_time_seconds = 0.0;

    std::string backend_name;
    std::string method_name;
    std::string message;

    bool success() const
    {
        return status == SolverStatus::Success;
    }
};

}
