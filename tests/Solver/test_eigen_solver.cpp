#include <OpenCAX/Solver/SolverFactory.h>
#include <Eigen/Sparse>
#include <iostream>
#include <vector>
int main(){
    OpenCAX::LinearSystem system; system.resize(3);
    std::vector<Eigen::Triplet<double>> t={{0,0,4},{0,1,-1},{1,0,-1},{1,1,4},{1,2,-1},{2,1,-1},{2,2,3}};
    system.A.setFromTriplets(t.begin(),t.end()); system.b<<15,10,10;
    OpenCAX::SolverOptions o; o.backend=OpenCAX::SolverBackend::Eigen; o.method=OpenCAX::SolverMethod::SparseLU;
    auto solver=OpenCAX::SolverFactory::create(o); auto r=solver->solve(system,o);
    std::cout<<"solver = "<<solver->name()<<"
"<<"success = "<<r.success()<<"
"<<"residual = "<<r.residual_norm<<"
"<<"x = "<<system.x.transpose()<<"
";
    return r.success()?0:1;
}
