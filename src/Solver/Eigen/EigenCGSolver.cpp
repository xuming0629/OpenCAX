#include <OpenCAX/Solver/Eigen/EigenCGSolver.h>

#include <Eigen/Sparse>
#include <Eigen/IterativeLinearSolvers>

#include <vector>


namespace OpenCAX
{


SolverResult
EigenCGSolver::solve(
    LinearSystem& system,
    const SolverOptions&
)
{

    SolverResult result;


    // ==================================================
    // OpenCAX CSR
    //        |
    //        v
    // Eigen SparseMatrix
    // ==================================================

    Eigen::SparseMatrix<double> A(
        system.A.rows(),
        system.A.cols()
    );


    std::vector<
        Eigen::Triplet<double>
    > triplets;



    for(int i = 0;
        i < system.A.rows();
        ++i)
    {

        for(
            int k = system.A.row_ptr[i];
            k < system.A.row_ptr[i + 1];
            ++k
        )
        {

            triplets.emplace_back(
                i,
                system.A.col_indices[k],
                system.A.values[k]
            );

        }

    }


    A.setFromTriplets(
        triplets.begin(),
        triplets.end()
    );



    // ==================================================
    // RHS
    // ==================================================

    Eigen::VectorXd b(
        system.b.size()
    );


    for(int i = 0;
        i < system.b.size();
        ++i)
    {
        b[i] = system.b[i];
    }



    // ==================================================
    // Eigen CG Solver
    // ==================================================

    Eigen::ConjugateGradient<
        Eigen::SparseMatrix<double>,
        Eigen::Lower | Eigen::Upper
    > solver;



    solver.compute(A);



    if(
        solver.info()!=Eigen::Success
    )
    {

        result.status =
            SolverStatus::Failed;

        result.message =
            "Eigen CG factorization failed";

        return result;

    }



    Eigen::VectorXd x =
        solver.solve(b);



    if(
        solver.info()!=Eigen::Success
    )
    {

        result.status =
            SolverStatus::Failed;

        result.message =
            "Eigen CG solve failed";

        return result;

    }



    // ==================================================
    // Copy Eigen result -> OpenCAX Vector
    // ==================================================

    system.x.resize(
        x.size()
    );


    for(int i=0;
        i < x.size();
        ++i)
    {

        system.x[i] = x[i];

    }



    // ==================================================
    // Result
    // ==================================================

    result.status =
        SolverStatus::Success;


    result.backend_name =
        "Eigen";


    result.method_name =
        "CG";


    result.iterations =
        solver.iterations();


    result.residual_norm =
        solver.error();



    result.message =
        "Eigen CG solve success";


    return result;

}


}