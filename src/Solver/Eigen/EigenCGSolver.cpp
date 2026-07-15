#include <OpenCAX/Solver/Eigen/EigenCGSolver.h>
#include <Eigen/IterativeLinearSolvers>
#include <chrono>
namespace OpenCAX {
SolverResult EigenCGSolver::solve(LinearSystem& system,const SolverOptions& o){
    SolverResult r; r.backend_name="Eigen"; r.method_name="ConjugateGradient";
    if(!system.valid()||system.size()==0){r.status=SolverStatus::InvalidSystem;r.message="Invalid or empty linear system";return r;}
    auto t0=std::chrono::steady_clock::now();
    Eigen::ConjugateGradient<LinearSystem::Matrix,Eigen::Lower|Eigen::Upper,Eigen::DiagonalPreconditioner<double>> s;
    s.setMaxIterations(o.max_iterations); s.setTolerance(o.tolerance); s.compute(system.A);
    if(s.info()!=Eigen::Success){r.message="CG matrix setup failed";return r;}
    system.x=s.solve(system.b); r.iterations=(int)s.iterations(); r.residual_norm=(system.A*system.x-system.b).norm(); double bn=system.b.norm(); r.relative_residual=bn>0?r.residual_norm/bn:r.residual_norm;
    r.solve_time_seconds=std::chrono::duration<double>(std::chrono::steady_clock::now()-t0).count();
    if(s.info()==Eigen::Success&&system.x.allFinite()){r.status=SolverStatus::Success;r.message="CG solve succeeded";}
    else if(r.iterations>=o.max_iterations){r.status=SolverStatus::MaxIterations;r.message="CG reached maximum iterations";}
    else {r.status=SolverStatus::Failed;r.message="CG solve failed";} return r;
}
} // namespace OpenCAX
