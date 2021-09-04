#ifndef SOLVER_9_1
#define SOLVER_9_1

#include "World.h"

class PotentialSolver
{
public:
    PotentialSolver(World &world, int max_it, double tol):
    world(world), max_solver_it(max_it), tolerance(tol) {}

    // solves potential using Gauss-Seidel
    bool solve();
    // comutes ElectricField = -grad(phi)
    void computeEF();

protected:
    World &world;
    unsigned max_solver_it;
    double tolerance;
};


#endif