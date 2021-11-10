#include <stdexcept>

#include "CommonIncl.h"
#include "ParticleSourceStock.h"

 
void particle_stock(
	const std::vector<ParticleStock>& particleStocks,	// params to delete particles
	const heap_array_md<double, Params::dim, Params::maxn>& x, // coordinates of all particles 
	heap_array<int, Params::maxn>& itype,	// material type of particles
	size_t& ntotal // number of particles
) {
	for (size_t i{}; i < ntotal; i++) {
		for (auto& stock : particleStocks) {
			if (stock.IsParticleInStock(std::array{ x(0, i), x(1, i) })) {
				itype(i) = 0;
			}
		}
	}
}

void particle_source(
	const std::vector<ParticleSource>& particleSources,	// params to spawn particles 
	heap_array_md<double, Params::dim, Params::maxn>& x,	// coordinates of all particles
	heap_array_md<double, Params::dim, Params::maxn>& vx,	// velocities of all particles
	heap_array<double, Params::maxn>& mass, // particle mass
	heap_array<double, Params::maxn>& rho, // particle densities
	heap_array<double, Params::maxn>& p,	 // particle pressure
	heap_array<double, Params::maxn>& u,	 // particle internal energy
	heap_array<int, Params::maxn>& itype,	// material type of particles
	size_t& ntotal  // number of particles 
) {
	size_t lastDeletedIndex{};
	size_t i{};
	for (auto& source : particleSources) {
		// try to fill in deleted particle
		for (; lastDeletedIndex < ntotal; lastDeletedIndex++) {
			if (itype(lastDeletedIndex) == 0) {
				i = lastDeletedIndex;
				break;
			}
		}

		// create particle
		for (size_t d{}; d < Params::dim; d++) {
			x(d, i) = source.x[d];
			vx(d, i) = source.vx[d];
		}
		rho(i) = source.rho;
		p(i) = source.p;
		u(i) = source.u;
		itype(i) = source.itype;
		mass(i) = source.mass;
		ntotal++;
		i++;
	}
}