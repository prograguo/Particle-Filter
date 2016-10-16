#ifndef __MOTION_MODEL__
#define __MOTION_MODEL__

#include "particle.h"
#include "str_io.h"

namespace str
{

// Class that takes is initialized with an initial estimate of odometry
// and is used to propagate any particle.
	class motion_model
	{
	public:

		motion_model(const params parameters);

		void propagate_particle(particle& particle);

		void update_odometry(const odom& reading1, const odom2& reading2);

	private:

		//Perturbation for the first rotation to align with the translation axis
		float theta1_rad = 0;

		//Perturbation for the translation along the translation axis
		float translation_cm = 0;

		//Perturbation for the final rotation to align with the final orientation
		float theta2_rad = 0;

		//Tuning parameters
		params params_;


	};


}//end namespace str


#endif