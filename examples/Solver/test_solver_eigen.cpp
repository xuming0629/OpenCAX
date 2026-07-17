#include <OpenCAX/Solver/SolverFactory.h>
#include <OpenCAX/Solver/SolverOptions.h>
#include <OpenCAX/Solver/LinearSystem.h>

#include <iostream>


using namespace OpenCAX;



// ============================================================
// 创建测试矩阵
//
// 4x + y = 1
// x + 3y = 2
//
// solution:
// x = 1/11
// y = 7/11
//
// ============================================================

static LinearSystem create_system()
{

    LinearSystem system;


    // -----------------------------
    // CSR Matrix
    //
    // [4 1]
    // [1 3]
    //
    // -----------------------------

    system.A.resize(
        2,
        2
    );


    system.A.row_ptr =
    {
        0,
        2,
        4
    };


    system.A.col_indices =
    {
        0,
        1,
        0,
        1
    };


    system.A.values =
    {
        4.0,
        1.0,
        1.0,
        3.0
    };



    // RHS

    system.b.resize(2);


    system.b[0]=1.0;

    system.b[1]=2.0;



    return system;

}





static void test_solver(
    SolverMethod method,
    const char* name
)
{

    std::cout
        << "\n============================\n";

    std::cout
        << "Testing "
        << name
        << "\n";


    std::cout
        << "============================\n";



    LinearSystem system =
        create_system();



    SolverOptions options;


    options.backend =
        SolverBackend::Eigen;


    options.method =
        method;


    options.verbose = true;



    auto solver =
        SolverFactory::create(
            options
        );



    std::cout
        << "Solver: "
        << solver->name()
        << "\n";



    SolverResult result =
        solver->solve(
            system,
            options
        );



    if(!result.success())
    {

        std::cout
            << "FAILED\n";

        return;

    }



    std::cout
        << "SUCCESS\n";


    std::cout
        << "solution:\n";


    for(int i=0;i<system.x.size();i++)
    {

        std::cout
            << "x["
            << i
            << "] = "
            << system.x[i]
            << "\n";

    }


}





int main()
{


    // ============================
    // Eigen SparseLU
    // ============================

    test_solver(
        SolverMethod::SparseLU,
        "Eigen SparseLU"
    );



    // ============================
    // Eigen CG
    // ============================

    test_solver(
        SolverMethod::CG,
        "Eigen CG"
    );



    // ============================
    // Eigen BiCGSTAB
    // ============================

    test_solver(
        SolverMethod::BiCGSTAB,
        "Eigen BiCGSTAB"
    );



    return 0;

}