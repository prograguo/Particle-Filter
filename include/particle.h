// Definition of the attributes of a particle

namespace str
{
	// Contain location and orientation of a particle
	struct particle
	{
		unsigned int x;
		unsigned int y;
		unsigned int theta;

		double weight;

		particle(unsigned int x_coord,unsigned int y_coord,unsigned int gamma)
		{
			x=x_coord;
			y=y_coord;
			theta=gamma;
			weight = 0;
		}
	};

	// Tuning parameters for the particle filter

	//TODO(ALL): Add tuning parameters for other parts of the filter
	struct params
	{
		//Parameters for the motion model
		float alpha1=0.1;
		float alpha2=0.1;
		float alpha3=0.1;
		float alpha4=0.1;
	};

}//end namespace str