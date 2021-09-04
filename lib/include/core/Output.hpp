#ifndef OUTPUT_9_1
#define OUTPUT_9_1

#include <vector>
#include <fstream>
#include "World.h"
#include "Sources.h"

namespace Output
{
    void fields(World &world, std::vector<Species> &species);
    void screenOutput(World &world, std::vector<Species> &species);
    void diagOutput(World &world, std::vector<Species> &species);
    void SimStartUp(World &world);
}

#endif