#ifndef __str__particle__
#define __str__particle__
// Definition of the attributes of a particle

namespace str
{
	// Contain location and orientation of a particle
	struct particle
	{
		unsigned int x_cm;
		unsigned int y_cm;
		float theta_deg;

		double weight;

		particle(unsigned int x_coord,unsigned int y_coord,unsigned int gamma)
		{
			x_cm=x_coord;
			y_cm=y_coord;
			theta_deg=gamma;
			weight = 0;
		}
	};

	// Tuning parameters for the particle filter

	//TODO(ALL): Add tuning parameters for other parts of the filter
	struct params
	{
		// Noise Parameters for the motion model
		float alpha1=0.1;
		float alpha2=0.1;
		float alpha3=0.1;
		float alpha4=0.1;
	};

}//end namespace str
#endif