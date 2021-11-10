#include "CommonIncl.h"
#include <string>
#include <fstream>
#include <iostream>

namespace {
	// save particle information to external disk file
	void print_human_text(
		const heap_array_md<double, Params::dim, Params::maxn>& x,	// coordinates of all particles
		const heap_array_md<double, Params::dim, Params::maxn>& vx,	// velocities of all particles
		const heap_array<double, Params::maxn>& mass,// particle masses
		const heap_array<double, Params::maxn>& rho,// density
		const heap_array<double, Params::maxn>& p,	// pressure
		const heap_array<double, Params::maxn>& u,	// specific internal energy
		const heap_array<double, Params::maxn>& c,	// sound velocity
		const heap_array<int, Params::maxn>& itype,	// material type
		const size_t ntotal,	// number of particles
		const size_t itimestep) // current time step
	{
		std::ofstream stream(std::to_string(itimestep) + ".txt");
		stream << "total: " << ntotal << std::endl;
		for (size_t i{}; i < ntotal; i++) {
			stream << i << std::endl;
			stream << "(" << x(0, i) << "; " << x(1, i) << ")\n";
			stream << "(" << vx(0, i) << "; " << vx(1, i) << ")\n";
			stream << mass(i) << " m" << std::endl;
			stream << rho(i) << " rho" << std::endl;
			stream << p(i) << " p" << std::endl;
			stream << u(i) << " u" << std::endl;
		}
	}

	// save particle information to external disk file
	void saveForSPH2D_Drawer(
		const heap_array_md<double, Params::dim, Params::maxn>& x,	// coordinates of all particles
		const heap_array_md<double, Params::dim, Params::maxn>& vx,	// velocities of all particles
		const heap_array<double, Params::maxn>& mass,// particle masses
		const heap_array<double, Params::maxn>& rho,// density
		const heap_array<double, Params::maxn>& p,	// pressure
		const heap_array<double, Params::maxn>& u,	// specific internal energy
		const heap_array<double, Params::maxn>& c,	// sound velocity
		const heap_array<int, Params::maxn>& itype,	// material type
		const size_t ntotal,	// number of particles
		const size_t itimestep) // current time step
	{
		std::ofstream stream;

		static bool isFirstStart = true;
		if (isFirstStart) {
			stream.open("params.txt", std::ofstream::out);
			// save params:
			stream << Params::x_mingeom << std::endl; // origin.X
			stream << Params::y_mingeom << std::endl; // origin.Y
			stream << Params::x_maxgeom - Params::x_mingeom << std::endl; // size.X
			stream << Params::y_maxgeom - Params::y_mingeom << std::endl; // size.Y
			stream << Params::dx << std::endl; // dx
			stream << Params::dy << std::endl; // dy
			isFirstStart = false;
		}
		stream.open("params.txt", std::ofstream::app);

		stream << ntotal << std::endl;
		for (size_t i{}; i < ntotal; i++) {
			stream << x(0, i) << std::endl << x(1, i) << std::endl; 
			stream << itype(i) << std::endl; 
		}
	} 

}

// save particle information to external disk file
void output(
	const heap_array_md<double, Params::dim, Params::maxn>& x,	// coordinates of all particles
	const heap_array_md<double, Params::dim, Params::maxn>& vx,	// velocities of all particles
	const heap_array<double, Params::maxn>& mass,// particle masses
	const heap_array<double, Params::maxn>& rho,// density
	const heap_array<double, Params::maxn>& p,	// pressure
	const heap_array<double, Params::maxn>& u,	// specific internal energy
	const heap_array<double, Params::maxn>& c,	// sound velocity
	const heap_array<int, Params::maxn>& itype,	// material type
	const size_t ntotal,	// number of particles
	const size_t itimestep) // current time step
{
	std::cout << itimestep << std::endl;
	saveForSPH2D_Drawer(x, vx, mass, rho, p, u, c, itype, ntotal, itimestep);
	//print_human_text(x, vx, mass, rho, p, u, c, itype, ntotal, itimestep);
}

