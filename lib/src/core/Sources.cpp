#include<math.h>
#include <core/Sources.h>
#include <core/Field.h>

void Species::advance()
{
    std::cout << "\tspcies advance"<< std::endl;
    double dt = world.getDt();

    v3d x0 = world.getOrigin();
    v3d xm = world.getMaxBound();

    for(Particle &part: particles)
    {
        v3d lc = world.XtoL(part.pos);
        // std::cout << "\tlc: " << lc << std::endl;
        
        v3d ef_part = world.ef.gather(lc);
        // std::cout << "\tef_part: " << ef_part << std::endl;

        // std::cout << "\tpart.vel - before" << part.vel << std::endl;
        part.vel += ef_part*(dt*charge/mass);
        // std::cout << "\tpart.vel - after" << part.vel << std::endl;

        // std::cout << "\tpart.pos - before" << part.vel << std::endl;
        part.pos += part.vel*dt;
        // std::cout << "\tpart.pos - after" << part.vel << std::endl;

        // if leaves domain reflect back
        for(int i=0;i<3;i++)
        {

            if(part.pos[i] < x0[i])
            {
                part.pos[i]  = 2*x0[i] - part.pos[i];
                part.vel[i] *= -1.0;
            }
            else if(part.pos[i]>=xm[i])
            {
                part.pos[i]  = 2*xm[i] - part.pos[i];
                part.vel[i] *= -1.0;
            }

        }
    }
}

void Species::computeNumberDensity()
{

    std::cout << "\tcomp num den"<< std::endl;
    den.clear();

    for(Particle &part : particles)
    {
        v3d lc = world.XtoL(part.pos);
        // std::cout << "\tlc: " << lc << std::endl << std::endl;
        den.scatter(lc, part.mpw);
    }

    den /= world.node_vol;
}

void Species::addParticle(v3d pos, v3d vel, double mpw)
{
    if(!world.inBounds(pos))
    {   
        std::cerr << "Out of Bounds particle" << std::endl;
        return;
    }
    v3d lc = world.XtoL(pos);

    v3d ef_part = world.ef.gather(lc);

    vel -= charge/mass * ef_part * (0.5 * world.getDt());

    particles.emplace_back(pos, vel, mpw);
}

void Species::loadParticlesBox(v3d x1, v3d x2, double num_den, int num_mp)
{
    auto tmp = (x2 - x1);
    double box_vol = tmp[0]*tmp[1]*tmp[2];
    double num_real = num_den * box_vol;
    double mpw = num_real/num_mp;

    particles.reserve(num_mp);

    for(int p=0; p<num_mp; p++)
    {
        v3d pos;
        double r1 = rnd(), r2 = rnd(), r3 = rnd();
        v3d rnd_vec = v3d(r1,r2,r3);

        pos = x1 + ( rnd_vec * (x2 - x1) );

        v3d vel {0,0,0};

        addParticle(pos, vel, mpw);
    }
}

void Species::loadParticlesBoxQS(v3d x1, v3d x2, double num_den, v3i num_mp)
{
    auto tmp = (x2 - x1);
    double box_vol = tmp[0]*tmp[1]*tmp[2];
    auto tmp1 = (num_mp - 1);
    int num_mp_tot = tmp[0]*tmp[1]*tmp[2];
    double num_real = num_den * box_vol;
    double mpw = num_real/num_mp_tot;

    v3d d = (x2 - x1) / tmp1;

    particles.reserve(num_mp_tot);

    for(int i=0; i<tmp1[0]; i++)
        for(int j=0; j<tmp1[1]; j++)
            for(int k=0; k<tmp1[2]; k++)
            {
                v3d coord {(double)i,(double)j,(double)k};

                v3d pos = x1 + (coord * d);

                for(int idx=0;idx<3;idx++)
                    if(pos[idx]==x2[idx])
                        pos[idx]-= 1.0e-4 * d[idx];

                double w = 1;   //relative weight
                if (i==0 || i==num_mp[0]-1) w*=0.5;
				if (j==0 || j==num_mp[1]-1) w*=0.5;
				if (k==0 || k==num_mp[2]-1) w*=0.5;

                // add rewind
                v3d vel {0,0,0};

                if(world.inBounds(pos))
                    addParticle(pos, vel, mpw*w);
            }
}

double Species::getRealParticleCount()
{
    double mpw_sum = 0;
    for(Particle &part : particles)
        mpw_sum += part.mpw;

    return mpw_sum;
}

v3d Species::getMomentum()
{
    v3d mom;
    for(Particle &part : particles)
        mom += part.mpw*part.vel;
    return mass * mom;
}

double Species::getKE()
{
    double ke=0;
    for(Particle &part: particles)
    {
        double v2 = part.vel.mag2();
        ke += part.mpw*v2;
    }
    return 0.5*mass*ke;
}