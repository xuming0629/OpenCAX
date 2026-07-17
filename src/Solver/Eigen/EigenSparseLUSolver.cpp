#include <OpenCAX/Solver/Eigen/EigenSparseLUSolver.h>

#include <Eigen/Sparse>
#include <Eigen/SparseLU>


namespace OpenCAX
{


SolverResult
EigenSparseLUSolver::solve(
    LinearSystem& system,
    const SolverOptions&
)
{

    SolverResult result;


    Eigen::SparseMatrix<double> A(
        system.A.rows(),
        system.A.cols()
    );


    std::vector<Eigen::Triplet<double>> triplets;


    for(int i=0;i<system.A.rows();++i)
    {

        for(
            int k=system.A.row_ptr[i];
            k<system.A.row_ptr[i+1];
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


    Eigen::SparseLU<
        Eigen::SparseMatrix<double>
    > solver;


    solver.compute(A);


    if(
        solver.info()!=Eigen::Success
    )
    {
        result.status = SolverStatus::Failed;
        return result;
    }



    Eigen::VectorXd b(system.b.size());


    for(int i=0;i<system.b.size();++i)
    {
        b[i]=system.b[i];
    }



    Eigen::VectorXd x =
        solver.solve(b);



    if(
        solver.info()!=Eigen::Success
    )
    {
        result.status = SolverStatus::Failed;
        return result;
    }



    system.x.resize(
        x.size()
    );


    for(int i=0;i<x.size();++i)
    {
        system.x[i]=x[i];
    }



    result.status =
        SolverStatus::Success;


    result.backend_name =
        "Eigen";


    result.method_name =
        "SparseLU";


    return result;

}


}