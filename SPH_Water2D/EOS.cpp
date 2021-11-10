#include "CommonIncl.h"
 

// artificial equation of state for the artificial compressibility
void p_art_water(
	const double rho, // density
	const double u,	// internal energy
	double& p, // pressure
	double& c) // sound velocity
{
	// artificial EOS, Form 1 (Monaghan, 1994)
	 static constexpr double gamma{ 7.0 };
	 static constexpr double rho0{ 1000.0 };
	 static constexpr double b{0.1};//{ 1.013e5 };
	 p = b * (pow(rho / rho0, gamma) - 1.0); 
	 
	 static constexpr double c0{0.01};//{ 1480.0 };
	 c = c0;

	// Artificial EOS, Form2 (Morris, 1997)
	//c = 0.04; 
	//p = sqr(c) * rho;
}