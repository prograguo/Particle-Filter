#ifndef __str__particle__
#define __str__particle__

#include <vector>
// Definition of the attributes of a particle

namespace str
{
	// Contain location and orientation of a particle
	struct particle
	{
		double x_cm;
		double y_cm;
		double theta_deg;

		double weight;

		particle(unsigned int x_coord,unsigned int y_coord,unsigned int gamma)
		{
			x_cm=x_coord;
			y_cm=y_coord;
			theta_deg=gamma;
			weight = 0;
		}
	};

	//Define a collection of particles 
	typedef std::vector<particle> particles;

}//end namespace str
#endif
