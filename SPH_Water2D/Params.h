#pragma once 
namespace Params {
	// dimension of the problem (1, 2, 3)
	constexpr int dim{ 2 };

	constexpr int maxn{ 24'000 }; // maximum number of particles
	constexpr int max_interaction{ 100 * maxn }; // maximum number of interaction pairs
	  
	//constexpr double x_maxgeom{ 0.002 }; // upper limit of allowed x-regime
	//constexpr double x_mingeom{ -0.000125 }; // lower limit of allowed x-regime
	//constexpr double y_maxgeom{ 0.001 }; // upper limit of allowed y-regime
	//constexpr double y_mingeom{ -0.000125 }; // lower limit of allowed y-regime  
	constexpr double x_maxgeom{ 0.002 };//1.e-3 / 38 }; // upper limit of allowed x-regime
	constexpr double x_mingeom{ -1.e-3 / 38 }; // lower limit of allowed x-regime
	constexpr double y_maxgeom{ 0.0015 }; // upper limit of allowed y-regime
	constexpr double y_mingeom{ -1.e-3 / 38 }; // lower limit of allowed y-regime 
	  
	// SPH algorithm for particle approximation
	// pa_sph = 1 : (p[i] + p[i])/(rho[i]*rho[j])
	//		    2 : p[i]/sqr(rho[i]) + p[j]/sqr(rho[j]
	constexpr int pa_sph{ 2 };

	// nearest neighboring particle searching method
	// nnps = 1 : simplest and direct searching
	//		  2 : sorting grid linked list
	//        3 : tree algorithm
	constexpr int nnps{ 1 };
	  
	// smoothing kernel function
	// skf = 1 : cubic spline W4 - Spline (Monaghan 1985)
	//		 2 : Gauss kernel (Gingold, Monaghan 1981)
	//		 3 : Quintic kernel (Morris 1997)
	constexpr int skf{ 3 };
	 

	// const smoothing length
	inline double hsml{ 1.e-3 / 40.0 };//{ 0.015 };

	// initial distance between particles
	constexpr double dx{ 1.e-3 / 40.0 };
	constexpr double dy{ 1.e-3 / 40.0 };

	/// Switches for diferent scenarios;
	
	// true : use density summation model
	// false : use continuity equation
	constexpr bool summation_density{ true };

	// true : Monaghan treatment on average velocity
	// false : no average treatment
	constexpr bool average_velocity{ true }; // Liu G.R. (eq 4.92)

	// true : load initial configuration data
	// false : generate initial configuration
	constexpr bool config_input{ false };

	// true : use virtual particle
	// false : no use of virtual particle
	constexpr bool virtual_part{ true };

	// true : load virtual particle information
	// false : generate virtual particle information
	constexpr bool vp_input{ false };

	// viscosity on?
	constexpr bool visc{ true };

	// external force on?
	constexpr bool ex_force{ true };

	// artificial viscosity on?
	constexpr bool visc_artificial{ true };

	// artificial heat on?
	constexpr bool heat_artificial{ false };

	// self gravity on?
	constexpr bool self_gravity{ true };

	// true : density normalization by using CSPM
	// false : no normalization
	constexpr bool nor_density{ false };


	// symmetry of the problem
	// nsym = 0 : no symmetry
	//		  1 : axis symmetry
	//		  2 : center symmetry
	constexpr int nsym{ 0 };



	/// control parameters for output

	// true : print statistics about SPH particle interactions including virtual particle information
	constexpr bool int_stat{ true };

	constexpr size_t maxtimestep{ 400 }; // time step to finish
	constexpr int print_step{ 100 }; // print timestep (on screen)
	constexpr int save_step{ 5 }; // save timestep (on disk)
	constexpr int moni_particle{ 1600 }; // num of particles for information monitoring

	constexpr double pi{ 3.14159265358979323846 };

	/// simulation cases

	constexpr bool shocktube{ false }; // true : shock tube simulation
	constexpr bool shearcavity{ true }; // true : shear cavity simulation
};