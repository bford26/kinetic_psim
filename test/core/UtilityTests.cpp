#include <iostream>
#include "Utils.hpp"

// testing v2d operations
// testing v3d operations
// testing v3d and v3i interactions 

using namespace std;

int main(int argc, char** argv)
{

    // all operators +-*/ += -= *= /=

    v3d x, x0, dh; 

    x = {1,1,1};
    x0 = {0.5,0.75,0.25};
    dh = {0.25,0.25,0.25};

    v3d res {2, 1, 3};

    v3d lc = ( x - x0 ) / dh;
    
    std::cout << "lc  : " << lc << std::endl;
    std::cout << "res : " << res << std::endl; 




    return 0;
}



