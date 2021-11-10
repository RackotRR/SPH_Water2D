#pragma once
#include "CommonIncl.h"
#include <array>
#include <vector>

// defines params to spawn one particle
struct ParticleSource {
	std::array<double, Params::dim> x;  // coordinates of particle spawn
	std::array<double, Params::dim> vx; // start particle velocities
	double mass;// start mass
	double rho; // start density
	double p;	// start pressure
	double u;	// start internal energy
	int itype;	// particle type

	ParticleSource(
		std::array<double, Params::dim>&& x,
		std::array<double, Params::dim>&& vx,
		double mass,
		double rho,
		double p,
		double u,
		int itype)
		:
		x{ std::move(x) },
		vx{ std::move(vx) },
		mass{ mass },
		rho{ rho },
		p{ p },
		u{ u },
		itype{ itype } {}
};

// defines cube, where particles will disappear
struct ParticleStock {
	std::array<double, Params::dim> origin;
	std::array<double, Params::dim> size;
	 
	ParticleStock(
		std::array<double, Params::dim>&& origin,
		std::array<double, Params::dim>&& size)
		:
		origin{ std::move(origin) },
		size{ std::move(size) } {}

	bool IsParticleInStock(const std::array<double, Params::dim>& x) const {
		for (size_t d{}; d < Params::dim; d++) {
			if (x[d] < origin[d] || x[d] > origin[d] + size[d]) {
				return false;
			}
		}
		return true;
	}
};

void particle_stock(
	const std::vector<ParticleStock>& particleStocks,	// params to delete particles
	const heap_array_md<double, Params::dim, Params::maxn>& x, // coordinates of all particles 
	heap_array<int, Params::maxn>& itype,	// material type of particles
	size_t& ntotal // number of particles
);

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
);