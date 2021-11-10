#include <iostream>

#include "CommonIncl.h"
#include "Input.h"
#include "Output.h"
#include "TimeIntegration.h" 

#include <RRTimeCounter/TimeCounter.h>

double sqr(double value) { 
	return value * value;
}

double pow(double value, unsigned power) {
	double result{ 1 };
	for (unsigned i{}; i < power; i++) {
		result *= value;
	}
	return result;
}  

void sph() {
	heap_array<double, Params::maxn> mass; // particle masses
	size_t ntotal; // number of particles
	double dt; // timestep
	heap_array<int, Params::maxn> itype;// material type of particles
	heap_array_md<double, Params::dim, Params::maxn> x;	// coordinates of all particles
	heap_array_md<double, Params::dim, Params::maxn> vx;// velocities of all particles
	heap_array<double, Params::maxn> rho; // density
	heap_array<double, Params::maxn> p;	// pressure
	heap_array<double, Params::maxn> u;	// specific internal energy
	heap_array<double, Params::maxn> c;	// sound velocity 
	heap_array<double, Params::maxn> e;	// total energy of particles
	std::vector<ParticleSource> particleSources;// params to spawn particles
	std::vector<ParticleStock> particleStocks;	// params to delete particles
	
	// double s1, s2;
	// time_print();
	// time_elapsed(s1);

	if (Params::shocktube) {
		dt = 0.005;
	}
	if (Params::shearcavity) {
		dt = 5e-5;
	}

	input(x, vx, mass, rho, p, u, itype, particleSources, particleStocks, ntotal);

	// input maxtimestep
	static constexpr size_t maxtimestep{ Params::maxtimestep };
	try {
		time_integration(x, vx, mass, rho, p, u, c, e, itype, particleSources, particleStocks, ntotal, maxtimestep, dt);
	}
	catch (std::exception& e) {
		std::cerr << e.what() << std::endl;
	}
	output(x, vx, mass, rho, p, u, c, itype, ntotal, maxtimestep);

	// time_print();
	// time_elapsed(s2);
	// elapsed cpu time = s2 - s1;
}
 

int main() {  
	TimeCounter<std::chrono::seconds> counter;
	std::cout << counter.Count([=]() {sph(); }) << std::endl;
	return 0;
}