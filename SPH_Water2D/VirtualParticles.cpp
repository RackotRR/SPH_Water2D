#include "CommonIncl.h"

// determine the information of virtual particles
// here only the Monaghan type virtual particles for the 2d shear
// cavity driven probles generated
void virt_part(
	const size_t itimestep, // current time step
	const size_t ntotal, // number of particles
	size_t& nvirt, // out, number of virtual particles 
	heap_array<double, Params::maxn>& mass,// out, particle masses
	heap_array_md<double, Params::dim, Params::maxn>& x,	// out, coordinates of all particles
	heap_array_md<double, Params::dim, Params::maxn>& vx,	// out, velocities of all particles
	heap_array<double, Params::maxn>& rho,	// out, density
	heap_array<double, Params::maxn>& u,	// out, specific internal energy
	heap_array<double, Params::maxn>& p,	// out, pressure
	heap_array<int, Params::maxn>& itype) // out, material type: 1 - ideal gas, 2 - water, 3 - tnt
{
	nvirt = 0;

	double dx{ Params::dx * 0.125 };
	double dy{ Params::dy * 0.125 };
	  
	// right border
	double minY{ Params::y_mingeom };
	if (itimestep > 1000) {
		minY = 0.5 * (Params::y_maxgeom + Params::y_mingeom);
	}
	for (double y{ minY }; y < Params::y_maxgeom; y += dy) {
		size_t i{ ntotal + nvirt };
		nvirt++;
		x(0, i) = Params::x_maxgeom * 0.5 - dx;
		x(1, i) = y;
	}
	
	// left border
	for (double y{ Params::y_mingeom }; y <= Params::y_maxgeom; y += dy) {
		size_t i{ ntotal + nvirt };
		nvirt++;
		x(0, i) = Params::x_mingeom + dx;
		x(1, i) = y;
	}
	 
	// right border
	for (double y{ Params::y_mingeom }; y < Params::y_maxgeom; y += dy) {
		size_t i{ ntotal + nvirt };
		nvirt++;
		x(0, i) = Params::x_maxgeom - dx;
		x(1, i) = y;
	}
	// bottom border
	for (double b{ Params::x_mingeom }; b < Params::x_maxgeom; b += dx) {
		size_t i{ ntotal + nvirt };
		nvirt++;
		x(0, i) = b;
		x(1, i) = Params::y_mingeom + dy; 
	}
	// bottom border
	for (double b{ Params::x_mingeom + 0.5 * dx }; b < Params::x_maxgeom; b += dx) {
		size_t i{ ntotal + nvirt };
		nvirt++;
		x(0, i) = b;
		x(1, i) = Params::y_mingeom + 2 * dy;
	} 
	// init all virtual particles
	for (size_t k{}; k < nvirt; k++) {
		size_t i{ ntotal + k };

		vx(0, i) = 0;
		vx(1, i) = 0;

		rho(i) = 1000;
		mass(i) = rho(i) * dx * dy;
		p(i) = 0;
		u(i) = 357.1;
		itype(i) = -2;
	}

	return;

	//size_t mp;
	//double xl, dx, v_inf;

	//if (Params::vp_input) {
	//	// open file xv_vp.dat
	//	// open file state_vp.dat
	//	// open file other_vp.dat
	//	// read nvirt
	//	// read everything else
	//	assert(false);
	//}
	//else {
	//	nvirt = 0;
	//	mp = 40;
	//	xl = 1.e-3;
	//	dx = xl / mp;
	//	v_inf = 1.e-3;

	//	// monaghan type virtual particle on the upper side
	//	for (size_t i{}; i < 2 * mp + 1; i++) {
	//		nvirt++;
	//		x(0, ntotal + nvirt) = i * dx * 0.5;
	//		x(1, ntotal + nvirt) = xl;
	//		vx(0, ntotal + nvirt) = v_inf;
	//		vx(1, ntotal + nvirt) = 0;
	//	}

	//	// monaghan type virtual particle on the lower side
	//	for (size_t i{}; i < 2 * mp + 1; i++) {
	//		nvirt++;
	//		x(0, ntotal + nvirt) = i * dx * 0.5;
	//		x(1, ntotal + nvirt) = 0;
	//		vx(0, ntotal + nvirt) = 0;
	//		vx(1, ntotal + nvirt) = 0;
	//	}

	//	// monaghan type virutal particle on the left side
	//	for (size_t i{}; i < 2 * mp - 1; i++) {
	//		nvirt++;
	//		x(0, ntotal + nvirt) = 0;
	//		x(1, ntotal + nvirt) = (i - 1) * dx * 0.5;
	//		vx(0, ntotal + nvirt) = 0;
	//		vx(1, ntotal + nvirt) = 0;
	//	}

	//	// monaghan type virtual particle on the right side
	//	for (size_t i{}; i < 2 * mp - 1; i++) {
	//		nvirt++;
	//		x(0, ntotal + nvirt) = xl;
	//		x(1, ntotal + nvirt) = (i - 1) * dx * 0.5;
	//		vx(0, ntotal + nvirt) = 0;
	//		vx(1, ntotal + nvirt) = 0;
	//	}

	//	for (size_t i{}; i < nvirt; i++) {
	//		rho(ntotal + i) = 1000;
	//		mass(ntotal + i) = rho(ntotal + i) * sqr(dx);
	//		p(ntotal + i) = 0;
	//		u(ntotal + i) = 357.1;
	//		itype(ntotal + i) = -2;
	////		hsml(ntotal + i) = dx;
	//	}
	//} 
}
