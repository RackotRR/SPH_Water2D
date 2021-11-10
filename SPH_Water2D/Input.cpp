#include "CommonIncl.h"
#include "Input.h"
  
// loading or generating initial particle information
void input(
	heap_array_md<double, Params::dim, Params::maxn>& x,	// coordinates of all particles
	heap_array_md<double, Params::dim, Params::maxn>& vx,	// velocities of all particles
	heap_array<double, Params::maxn>& mass,// particle masses
	heap_array<double, Params::maxn>& rho, // particle densities
	heap_array<double, Params::maxn>& p,	 // particle pressure
	heap_array<double, Params::maxn>& u,	 // particle internal energy
	heap_array<int, Params::maxn>& itype,	  // particle material type
	std::vector<ParticleSource>& particleSources,	// params to spawn particles
	std::vector<ParticleStock>& particleStocks,	// params to delete particles 
	size_t& ntotal) // total particle number
{ 
	ntotal = 0;

	double x0{ Params::x_mingeom };
	double xN{ Params::x_maxgeom };
	double y0{ Params::y_mingeom };
	double yN{ Params::y_maxgeom };

	double dx{ Params::dx };
	double rho0 = 1000;
	double mass0 = dx * dx * rho0;
	double p0 = 0;
	double u0 = 357.1;
	int itype0 = 2; // water
	  
	shear_cavity(x, vx, mass, rho, p, u, itype, ntotal);
	/*for (size_t i{}; i < 10; i++) {
		std::array x{ (x0 + xN) * 0.5 + dx * i, (y0 + yN) * 0.5 };
		std::array vx{ 0.0, 0.0 };
		ParticleSource source{ std::move(x), std::move(vx), mass0, rho0, p0, u0, itype0 };
		particleSources.push_back(source);
	}*/
	return;

	// open ini_xv.dat
	// open ini_state.dat
	// open ini_other.dat
	if (Params::config_input) {
		// loading initial particle configuration...
		// load params from keyboard
	}
	else {
		if (Params::shocktube) {
			shock_tube(x, vx, mass, rho, p, u, itype, ntotal);
		}
		if (Params::shearcavity) {
			shear_cavity(x, vx, mass, rho, p, u, itype, ntotal);
		}

		//for (size_t k{}; k < ntotal; k++) {
		//	// write info
		//}
		//// write info
	} 
}

// generate data for 1d shock tube problem
void shock_tube(
	heap_array_md<double, Params::dim, Params::maxn>& x,	// coordinates of all particles
	heap_array_md<double, Params::dim, Params::maxn>& vx,	// velocities of all particles
	heap_array<double, Params::maxn>& mass,// particle masses
	heap_array<double, Params::maxn>& rho, // particle densities
	heap_array<double, Params::maxn>& p,	 // particle pressure
	heap_array<double, Params::maxn>& u,	 // particle internal energy
	heap_array<int, Params::maxn>& itype,	  // particle material type
	size_t& ntotal) // total particle number
{ 
	ntotal = 400;
	static constexpr double space_x{ 0.6 / 80 };

	for (size_t i{}; i < ntotal; i++) {
		mass(i) = 0.75 / 400;
		Params::hsml = 0.015;
		itype(i) = 1; // ideal gas
		for (size_t d{}; d < Params::dim; d++) {
			x(d, i) = 0;
			vx(d, i) = 0;
		}
	}

	for (size_t i{}; i < 320; i++) {
		x(0, i) = -0.6 + space_x / 4.0 * i;
	}  
	for (size_t i{ 320 }; i < ntotal; i++) {
		x(0, i) = 0.0 + space_x * (i - 320);
	}

	for (size_t i{}; i < ntotal; i++) {
		if (x(0, i) <= 1.e-8) {
			u(i) = 2.5;
			rho(i) = 1;
			p(i) = 1;
		}
		if (x(0, i) > 1.e-8) {
			u(i) = 1.795;
			rho(i) = 0.25;
			p(i) = 0.1795;
		}
	}
}

// generate data for the 2d shear driven cavity problem with Re=1
void shear_cavity(
	heap_array_md<double, Params::dim, Params::maxn>& x,	// coordinates of all particles
	heap_array_md<double, Params::dim, Params::maxn>& vx,	// velocities of all particles
	heap_array<double, Params::maxn>& mass,// particle masses
	heap_array<double, Params::maxn>& rho, // particle densities
	heap_array<double, Params::maxn>& p,	 // particle pressure
	heap_array<double, Params::maxn>& u,	 // particle internal energy
	heap_array<int, Params::maxn>& itype,	 // particle material type
	size_t& ntotal) // total particle number
{  
	static constexpr size_t m{ 41 }, n{ 41 };
	static constexpr size_t mp{ m - 1 }, np{ n - 1 };
	ntotal = mp * np;
	static constexpr double xl{ 1.e-3 }, yl{ 1.e-3 };
	static constexpr double dx{ xl / mp }, dy{ yl / np };
	  
	for (size_t i{}; i < mp; i++) {
		for (size_t j{}; j < np; j++) {
			size_t k{ j + i * np };
			x(0, k) = i * dx + dx * 0.5;
			x(1, k) = j * dy + dy * 0.5;
		}
	}

	for (size_t i{}; i < ntotal; i++) {
		vx(0, i) = 0;
		vx(1, i) = 0;
		rho(i) = 1000;
		mass(i) = dx * dy * rho(i);
		p(i) = 0;
		u(i) = 357.1;
		itype(i) = 2; // water
	}

	Params::hsml = dx;
}