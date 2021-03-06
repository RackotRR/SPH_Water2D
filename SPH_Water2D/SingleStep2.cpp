#include "CommonIncl.h"
#include "VirtualParticles.h"
#include "DirectFind.h"
#include "GridFind.h"
#include "Density.h"
#include "Viscosity.h"
#include "InternalForce.h"
#include "ArtificialViscosity.h"
#include "ExtForce.h" 
#include "ArtificialHeat.h"
#include "AverageVelocity.h"
#include "ParticleSourceStock.h"

#include "Output.h"

// determine the right hand side of a differential equation
// in a single step for performing integration
void single_step(
	const size_t itimestep, // current timestep number
	const double dt, // timestep 
	const std::vector<ParticleSource>& particleSources,	// params to spawn particles
	const std::vector<ParticleStock>& particleStocks,	// params to delete particles
	size_t& ntotal, // number of particles
	heap_array<double, Params::maxn>& mass,// particle masses
	heap_array_md<double, Params::dim, Params::maxn>& x,	// coordinates of all particles
	heap_array_md<double, Params::dim, Params::maxn>& vx,	// velocities of all particles
	heap_array<double, Params::maxn>& u,	// specific internal energy 
	heap_array<double, Params::maxn>& rho,	// out, density
	heap_array<double, Params::maxn>& p,	// out, pressure
	heap_array<double, Params::maxn>& tdsdt,// out, production of viscous entropy t * ds/dt
	heap_array_md<double, Params::dim, Params::maxn>& dx,	// out, dx = vx = d(x)/dt
	heap_array_md<double, Params::dim, Params::maxn>& dvx,	// out, dvx = d(vx)/dt, force per unit mass
	heap_array<double, Params::maxn>& du,	// out, du = d(u)/dt
	heap_array<double, Params::maxn>& drho,	// out, drho = d(rho)/dt
	heap_array<int, Params::maxn>& itype,// material type of particles
	heap_array_md<double, Params::dim, Params::maxn>& av) // out, Monaghan average velocity
{
	size_t nvirt{}, niac{};
	heap_array<size_t, Params::max_interaction> pair_i, pair_j;
	heap_array<double, Params::max_interaction> w;
	heap_array_md<double, Params::dim, Params::max_interaction> dwdx;
	heap_array_md<double, Params::dim, Params::maxn> indvxdt, exdvxdt, arvdvxdt;
	heap_array<double, Params::maxn> avdudt, ahdudt, c, eta;

	//output(x, vx, mass, rho, p, u, c, itype, ntotal, itimestep);

	// particle stock function 
	particle_stock(particleStocks, x, itype, ntotal);
	static bool once{ false };
	if (!once && itimestep < 500 && itimestep % 10 == 0) {
		// particle source function
		once = true;
		particle_source(particleSources, x, vx, mass, rho, p, u, itype, ntotal);
	}

	// positions of virtual (boundary) particles 
	if (Params::virtual_part) {
		virt_part(itimestep, ntotal, nvirt, mass, x, vx, rho, u, p, itype);
	}

	// interaction parameters, calculating neighboring particles and optimizing smoothing lenght
	if (Params::nnps == 1) {
		direct_find(ntotal + nvirt, x, itype, niac, pair_i, pair_j, w, dwdx);
	}
	else if (Params::nnps == 2) {
		grid_find(ntotal + nvirt, x, itype, niac, pair_i, pair_j, w, dwdx);
	}
	else if (Params::nnps == 3) {
		//tree_search(ntotal + nvirt, x, niac, pair_i, pair_j, w, dwdx);
		assert(false);
	}
	else {
		assert(false);
	}


	// density approximation or change rate
	if (Params::summation_density) {
		sum_density(ntotal + nvirt, mass, x, niac, pair_i, pair_j, w, itype, rho);
	} 
	else {
		con_density(ntotal + nvirt, mass, x, vx, niac, pair_i, pair_j, w, itype, rho, dwdx, drho);
	}

	// dynamic viscosity
	if (Params::visc) {
		viscosity(ntotal + nvirt, itype, x, rho, eta);
	}

	// internal forces
	int_force(itimestep, dt, ntotal + nvirt, mass, vx, niac, rho, eta, pair_i, pair_j, dwdx,
		itype, u, x, c, p, indvxdt, tdsdt, du);

	// artificial viscosity
	if (Params::visc_artificial) {
		art_visc(ntotal + nvirt, mass, x, vx, niac, rho, c, pair_i, pair_j, w, dwdx, arvdvxdt, avdudt);
	}

	// external forces
	if (Params::ex_force) {
		ext_force(ntotal + nvirt, mass, x, niac, pair_i, pair_j, itype, exdvxdt);
	} 

	if (Params::heat_artificial) {
		art_heat(ntotal + nvirt, mass, x, vx, niac, rho, u, c, pair_i, pair_j, w, dwdx, ahdudt);
	}

	// calculating average velocity of each particle for avoiding penetration
	if (Params::average_velocity) {
		av_vel(ntotal, mass, vx, niac, rho, pair_i, pair_j, w, av);
	}

	// convert velocity, force and energy to f and dfdt
	for (size_t i{}; i < ntotal; i++) { 
		for (size_t d{}; d < Params::dim; d++) {
			dvx(d, i) = indvxdt(d, i) + exdvxdt(d, i) + arvdvxdt(d, i);
		}
		du(i) += avdudt(i) + ahdudt(i);
	}  
}