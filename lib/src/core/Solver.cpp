#include <core/Solver.h>
#include <core/Field.h>
#include <math.h>
#include <iostream>
#include <core/World.h>
#include <core/constants.hpp>

using namespace core::constants;

bool PotentialSolver::solve()
{
    Field &phi = world.phi;
    Field &rho = world.rho;

    // pre compute
    v3d dh = world.getCellSpacing();
    double idx2 = 1.0 / (dh[0]*dh[0]);
    double idy2 = 1.0 / (dh[1]*dh[1]);
    double idz2 = 1.0 / (dh[2]*dh[2]);

    double L2 = 0;              // L2 norm
    bool converged = false;

    // solver action
    for (unsigned it = 0; it < max_solver_it; it++)
    {
        
        for(int i=1; i<world.ni-1; i++)
            for(int j=1; j<world.nj-1; j++)
                for(int k=1; k<world.nk-1; k++)
                {
                    double phi_new = (1);

                    auto t0 = rho[i][j][k] / EPS0;
                    auto tx = idx2 * (phi[i-1][j][k] + phi[i+1][j][k]);
                    auto ty = idy2 * (phi[i][j-1][k] + phi[i][j+1][k]);
                    auto tz = idz2 * (phi[i][j][k-1] + phi[i][j][k+1]);
                    auto t1 = 2 * (idx2 + idy2 + idz2);

                    phi_new = ( t0 + tx + ty + tz ) / t1;

                    // SOR
                    phi[i][j][k] = phi[i][j][k] + SQRT2 * (phi_new - phi[i][j][k]);
                }

        // check convergence
        if(it % 25 == 0)
        {
            double sum=0;

            for (int i=1;i<world.ni-1;i++)
				for (int j=1;j<world.nj-1;j++)
					for (int k=1;k<world.nk-1;k++)
					{
						double R = -phi[i][j][k]*(2*idx2+2*idy2+2*idz2) +
									rho[i][j][k]/EPS0 +
									idx2*(phi[i-1][j][k] + phi[i+1][j][k]) +
									idy2*(phi[i][j-1][k]+phi[i][j+1][k]) +
									idz2*(phi[i][j][k-1]+phi[i][j][k+1]);

						sum += R*R;
					}

            L2  = sqrt( sum / (world.ni*world.nj*world.nk));
            if(L2 < tolerance) {converged = true; break;}
        }
    }

    if(!converged) std::cerr << "GS failed to converge, L2=" << L2 << std::endl;
    return converged;
}

void PotentialSolver::computeEF()
{

    Field &phi = world.phi;

    v3d dh = world.getCellSpacing();

    double dx = dh[0];
    double dy = dh[1];
    double dz = dh[2];

    for(int i=0; i<world.ni; i++)
        for(int j=0; j<world.nj; j++)
            for(int k=0; k<world.nk; k++)
            {
                v3d &ef = world.ef[i][j][k];

                // x component
                if(i==0)
                    ef[0] = -(-3*phi[i][j][k]+4*phi[i+1][j][k]-phi[i+2][j][k])/(2*dx);
                else if(i==world.ni-1)
                    ef[0] = -(phi[i-2][j][k]-4*phi[i-1][j][k]+3*phi[i][j][k])/(2*dx);
                else
                    ef[0] = - (phi[i+1][j][k] - phi[i-1][j][k]) / (2*dx);

                // y component
                if (j==0)
					ef[1] = -(-3*phi[i][j][k] + 4*phi[i][j+1][k]-phi[i][j+2][k])/(2*dy);
				else if (j==world.nj-1)
					ef[1] = -(phi[i][j-2][k] - 4*phi[i][j-1][k] + 3*phi[i][j][k])/(2*dy);
				else
					ef[1] = -(phi[i][j+1][k] - phi[i][j-1][k])/(2*dy);

                // z component
                if (k==0)
					ef[2] = -(-3*phi[i][j][k] + 4*phi[i][j][k+1]-phi[i][j][k+2])/(2*dz);
				else if (k==world.nk-1)
					ef[2] = -(phi[i][j][k-2] - 4*phi[i][j][k-1]+3*phi[i][j][k])/(2*dz);
				else
					ef[2] = -(phi[i][j][k+1] - phi[i][j][k-1])/(2*dz);
            }
}




