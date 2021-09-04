#include<iostream>
#include<vector>

#include<core/constants.hpp>
#include<core/World.h>
#include<core/Field.h>
#include<core/Output.hpp>
#include<core/Sources.h>
#include<core/Solver.h>

using namespace core::constants;

int main(int argc, char** argv)
{

    // setup domain
    World world(21,21,21);
    world.setExtents({-0.1,-0.1,0},{0.1,0.1,0.2});
    world.setTime(2e-10, 10000);

    // screen output environment specs
    Output::SimStartUp(world);    

    // setup species
    std::vector<Species> species;
    species.reserve(2);
    species.push_back(Species("O+", 16*AMU, Qe, world));    // oxygen ions
    species.push_back(Species("e-", Me, -1*Qe, world));     // electrons 

    std::cout << "Size of species " << species.size() << std::endl << std::endl;

    // create particles
    // v3i np_ions_grid = {21,21,21};
    // v3i np_eles_grid = {21,21,21};
    // species[0].loadParticlesBoxQS(world.getOrigin(), world.getMaxBound(), 1e11, np_ions_grid);
    // species[1].loadParticlesBoxQS(world.getOrigin(), world.getCentroid(), 1e11, np_eles_grid);

    // add single particles work way up to loadPartBox
    // species[0].particles.reserve(1);
    // species[1].particles.reserve(1);

    species[0].addParticle(world.getCentroid(), {0,0,0}, 1);
    species[1].addParticle(world.getCentroid(), {0,0,0}, 1);

    // std::cout << "O+ Position: " << species[0].particles[0].pos << std::endl; 


    // init solver and solve initial potential
    PotentialSolver solver(world, 10000, FLT_EPS);
    solver.solve();

    // get init e-field
    solver.computeEF();

    // if no particles dont run simulation loop
    bool runSimulation = true;
    

    // std::cout << "species[0] particles: " << species[0].particles.size() << std::endl;
    // std::cout << "species[1] particles: " << species[1].particles.size() << std::endl;

    if(runSimulation)
    {
        // simulation loop
        while(world.advanceTime())
        {
            // move particles
            std::cout<< "Moving parts\n";
            for(Species &sp: species)
            {
                sp.advance();
                sp.computeNumberDensity();
            }

            // compute charge density
            std::cout<< "Comp Charge Density\n";
            world.computeChargeDensity(species);

            // update potential
            std::cout<< "solver.solve()\n";
            solver.solve();

            // get new e-field
            std::cout<< "solver.computeEF()\n";
            solver.computeEF();

            // screen and file outputs
            Output::screenOutput(world, species);
            // Output::diagOutput(world, species);

            // condition output field data
            if(world.getCurrentTimeStep()%100 == 0 || world.isLastTimeStep())
                Output::fields(world, species);
        }
        std::cout << "Simulation took " << world.getWallTime() << " seconds";
        return 0;
    }
    else
    {
        std::cerr << "\nError: Simulation was not ran because no particles found..\n\n";
        return -1;
    }
}





// electrostatic sim

// solve init potential
// compute e field
// load particles

// simulation loop

// -- fields
// compute charge density
// compute potential
// compute electric field

// -- particles
// update particle velocity
// update particle position

// -- optional steps
// perform collisions
// inject additional particles
// output diagnostics

// output results