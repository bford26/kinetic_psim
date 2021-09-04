#include <math.h>
#include <core/World.h>
#include <core/Sources.h>
#include <core/constants.hpp>

Rnd rnd;

World::World(int ni, int nj, int nk):
    ni(ni), nj(nj), nk(nk), nn(ni,nj,nk),
    phi(ni,nj,nk), rho(ni,nj,nk), node_vol(ni,nj,nk),
    ef(ni,nj,nk)
{
    time_start = std::chrono::high_resolution_clock::now();
}

bool World::inBounds(v3d pos)
{
    // for(int i=0; i<3; i++)
    // {
    //     if( pos[i]<x0[i] || pos[i]>=xm[i]) return false;
    // }
    // return true;
    return ( (pos[0]>x0[0] || pos[0]<xm[0]) || (pos[1]>x0[1] || pos[1]<xm[1]) ||  ( pos[2]>x0[2] || pos[2]<xm[2]) );
}

void World::setExtents(v3d _x0, v3d _xm)
{
    x0 = _x0;
    xm = _xm;

    // cell spacing
    for (int i=0; i<3; i++)
        dh[i] = (xm(i)-x0(i))/(nn(i)-1);
    // dh = (xm-x0) / (nn - 1);

    // centroid
    xc = 0.5 * (x0 + xm);

    computeNodeVolumes();    
}

double World::getWallTime()
{
    auto time_now = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_delta = time_now-time_start;
    return time_delta.count();
}

void World::computeChargeDensity(std::vector<Species>& species)
{
    rho = 0;

    for(Species &sp : species)
    {
        if(sp.charge==0) continue;
        rho += sp.den * sp.charge;
    }
}

void World::computeNodeVolumes()
{
    for (int i=0;i<ni;i++)
		for (int j=0;j<nj;j++)
			for (int k=0;k<nk;k++)
			{
				double V = dh[0]*dh[1]*dh[2];	//default volume
				
                if (i==0 || i==ni-1) V*=0.5;	//reduce by two for each boundary index
				if (j==0 || j==nj-1) V*=0.5;
				if (k==0 || k==nk-1) V*=0.5;
				
                node_vol[i][j][k] = V;
			}
}

double World::getPE()
{
    double pe = 0;
    for (int i=0;i<ni;i++)
		for (int j=0;j<nj;j++)
			for (int k=0;k<nk;k++)
			{
				// v3d efn = ef[i][j][k];
                // double ef2 = efn.mag2();
                // pe += ef2*node_vol[i][j][k];
                // or just 
                pe += ef[i][j][k].mag2() * node_vol[i][j][k];
			}

    return 0.5 * core::constants::EPS0 * pe;
}



