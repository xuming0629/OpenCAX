#include <OpenCAX/Solver/SolverFactory.h>
#include <OpenCAX/Solver/Eigen/EigenSparseLUSolver.h>
#include <OpenCAX/Solver/Eigen/EigenCGSolver.h>
#include <OpenCAX/Solver/Eigen/EigenBiCGSTABSolver.h>
#include <stdexcept>
namespace OpenCAX {
std::unique_ptr<LinearSolver> SolverFactory::create(const SolverOptions& o){
    if(o.backend!=SolverBackend::Eigen) throw std::invalid_argument("SolverFactory: requested backend is not enabled");
    switch(o.method){
        case SolverMethod::SparseLU:return std::make_unique<EigenSparseLUSolver>();
        case SolverMethod::CG:return std::make_unique<EigenCGSolver>();
        case SolverMethod::BiCGSTAB:return std::make_unique<EigenBiCGSTABSolver>();
        default:throw std::invalid_argument("SolverFactory: unsupported Eigen solver method");
    }
}
} // namespace OpenCAX
