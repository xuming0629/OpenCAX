#include <OpenCAX/Solver/Eigen/EigenSparseLUSolver.h>
#include <Eigen/SparseLU>
#include <chrono>
namespace OpenCAX {
SolverResult EigenSparseLUSolver::solve(LinearSystem& system,const SolverOptions&) {
    SolverResult r; r.backend_name="Eigen"; r.method_name="SparseLU";
    if(!system.valid()||system.size()==0){r.status=SolverStatus::InvalidSystem;r.message="Invalid or empty linear system";return r;}
    auto t0=std::chrono::steady_clock::now();
    Eigen::SparseLU<LinearSystem::Matrix> s; s.analyzePattern(system.A);
    if(s.info()!=Eigen::Success){r.message="SparseLU analyzePattern failed";return r;}
    s.factorize(system.A); if(s.info()!=Eigen::Success){r.message="SparseLU factorization failed";return r;}
    system.x=s.solve(system.b); if(s.info()!=Eigen::Success||!system.x.allFinite()){r.message="SparseLU solve failed";return r;}
    r.residual_norm=(system.A*system.x-system.b).norm(); double bn=system.b.norm(); r.relative_residual=bn>0?r.residual_norm/bn:r.residual_norm;
    r.solve_time_seconds=std::chrono::duration<double>(std::chrono::steady_clock::now()-t0).count();
    r.status=SolverStatus::Success; r.message="SparseLU solve succeeded"; return r;
}
} // namespace OpenCAX
