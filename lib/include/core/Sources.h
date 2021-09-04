#ifndef SOURCES_9_1
#define SOURCES_9_1

#include <vector>
#include "Field.h"
#include "World.h"

template<class T>
struct Particle_
{
    T pos;
    T vel;
    double mpw;

    Particle_( T x, T v, double mpw): pos(x), vel(v), mpw(mpw) {}
};

// typedef Particle_<v2d> Particle2D;
typedef Particle_<v3d> Particle;

class Species
{
public:

    Species(std::string name, double mass, double charge, World& world) : name(name), mass(mass), charge{charge}, den(world.ni, world.nj, world.nk), world{world} {}

    size_t getSimulatedParticleCount() {return particles.size();}
    double getRealParticleCount();

    v3d getMomentum();

    double getKE();

    void advance();

    void computeNumberDensity();

    void addParticle(v3d pos, v3d vel, double mpwt);

    // random load of particles in a x1-x2 box
    void loadParticlesBox(v3d x1, v3d x2, double num_den, int num_mp);

    // quiet start load of particles in a x1-x2 box
    void loadParticlesBoxQS(v3d x1, v3d x2, double num_den, v3i num_mp);  
    
    const std::string name;             // speices name
    const double mass;                  // particle mass in kg
    const double charge;                // particle charge in Coulomb

    std::vector<Particle> particles;    
    Field den;                          // number density

protected:
    World &world;
};


#endif