#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include <random>
#include <chrono>
#include <iostream>
#include "Field.h"

class Species;

class Rnd
{
public:
    Rnd() : mt_gen{std::random_device()()}, rnd_dist{0,1.0} {}
    double operator() () {return rnd_dist(mt_gen);}

protected:
    std::mt19937 mt_gen;                                //random number generator
    std::uniform_real_distribution<double> rnd_dist;    //uniform dist
};

// just to tell compiler that object of type Rnd called rnd is defined somewhere
extern Rnd rnd;

class World
{
public:

    World(int ni, int nj, int nk);

    void setExtents(const v3d x0, const v3d xm);

    v3d getOrigin()         const {return x0;}
    v3d getMaxBound()       const {return xm;}
    v3d getCentroid()       const {return xc;}
    v3d getCellSpacing()    const {return dh;}

    int getCurrentTimeStep()    const {return ts;}
    double getPhysicalTime()    const {return time;}
    double getWallTime();
    double getDt()              const {return dt;}
    bool isLastTimeStep()       const {return ts==num_ts-1;}

    bool inBounds(v3d pos);

    void setTime(double dt, int num_ts) {this->dt=dt; this->num_ts=num_ts;}

    bool advanceTime() {time+=dt; ts++; return ts<=num_ts;}

    // convert phys space pos to logical coord
    v3d XtoL(v3d x) const
    {
        v3d lc = ( x - x0 ) / dh;

        if(lc[0] < ni && lc[1] < nj && lc[2] < nk)
            return lc;
        else
        {
            std::cerr << "Error: Passed logic Coord doesnt work: " << lc << std::endl;
            return {0,0,0};
        }
            
    }

    void computeChargeDensity(std::vector<Species> &species);

    double getPE();

    // mesh geometry
    const int ni, nj, nk; // num of nodes
    const v3i nn;

    Field phi;              // potential
    Field rho;              //charge density
    Field node_vol;         // cell volumes
    Field3 ef;              // electric field components

protected:

    v3d x0;         // mesh origin 
    v3d dh;         // cell spacing
    v3d xm;         // max bound corner opposite to x0
    v3d xc;         // domain cnetroid

    double dt = 0;      // time step length
    double time = 0;    // physical time
    int ts = -1;        // current time step
    int num_ts = 0;     // number of time steps

    std::chrono::time_point<std::chrono::high_resolution_clock> time_start; // time at sim start

    void computeNodeVolumes();
};

#endif
