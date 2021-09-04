#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <core/Output.hpp>

void Output::fields(World &world, std::vector<Species> &species)
{
    std::stringstream name;
    name << "../results/fields_"<<std::setfill('0')<<std::setw(5)<<world.getCurrentTimeStep()<<".vti";

    // open output file
    std::ofstream out(name.str());
    if(!out.is_open()) {std::cerr<<"Could not open " <<name.str()<<std::endl;return;}

    // imagedata is vtk format for structured caresian meshes
    out<<"VTKFile type=\"ImageData\">\n";
    v3d x0 = world.getOrigin();
    v3d dh = world.getCellSpacing();
    out<<"<ImageData Origin=\""<< x0[0] <<" "<< x0[1] <<" "<< x0[2]<<"\" ";
    out<<"Spacing=\""<<dh[0]<<" "<<dh[1]<<" "<<dh[2]<<"\" ";
    out<<"WholeExtent=\"0 "<<world.ni-1<<" 0 "<<world.nj-1<<" 0 "<<world.nk-1<<"\">\n";

    // output data on nodes (points)
    out<<"<PointData>\n";

	/*node volumes, scalar*/
	out<<"<DataArray Name=\"NodeVol\" NumberOfComponents=\"1\" format=\"ascii\" type=\"Float64\">\n";
	out<<world.node_vol;
	out<<"</DataArray>\n";

	/*potential, scalar*/
	out<<"<DataArray Name=\"phi\" NumberOfComponents=\"1\" format=\"ascii\" type=\"Float64\">\n";
	out<<world.phi;
	out<<"</DataArray>\n";

	/*charge density, scalar*/
	out<<"<DataArray Name=\"rho\" NumberOfComponents=\"1\" format=\"ascii\" type=\"Float64\">\n";
	out<<world.rho;
	out<<"</DataArray>\n";

    for (Species &sp:species)
	{
		out<<"<DataArray Name=\"nd."<<sp.name<<"\" NumberOfComponents=\"1\" format=\"ascii\" type=\"Float64\">\n";
		out<<sp.den;
		out<<"</DataArray>\n";
	}


    out<<"<DataArray Name=\"ef\" NumberOfComponents=\"3\" format=\"ascii\" type=\"Float64\">\n";
	out<<world.ef;
	out<<"</DataArray>\n";

    out<<"</PointData>\n";
	out<<"</ImageData>\n";
	out<<"</VTKFile>\n";
 	out.close();
}

void Output::screenOutput(World &world, std::vector<Species> &species)
{
    std::cout<<"ts: "<<world.getCurrentTimeStep();
	for (Species &sp:species)
		std::cout<<std::setprecision(3)<<"\t "<<sp.name<<":"<<sp.getSimulatedParticleCount();
	std::cout<<std::endl;
}

namespace Output
{
    std::ofstream f_diag;
}

void Output::diagOutput(World &world, std::vector<Species> &species)
{

}


void Output::SimStartUp(World &world)
{

	std::cout << "Min:    " << world.getOrigin() << std::endl;
	std::cout << "Max:    " << world.getMaxBound() << std::endl;
	std::cout << "dh:     " << world.getCellSpacing() << std::endl;
	std::cout << "dt:     " << world.getDt() << std::endl;
	std::cout << std::endl;

}
