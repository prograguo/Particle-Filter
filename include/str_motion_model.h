#ifndef __MOTION_MODEL__
#define __MOTION_MODEL__

#include "particle.h"
#include "str_io.h"

namespace motion_model
{

// Class that takes is initialized with an initial estimate of odometry
// and is used to propagate any particle.
	class motion_propagation
	{
	public:

		motion_propagation(const odom& inital_reading);

		void propagate_particle(particle& original_particle);

		void update_odometry(const odom& reading1, const odom2& reading2);

	private:

		//First rotation to align with the translation axis
		float theta1_ = 0;

		//Translation along the translation axis
		float translation_ = 0;

		//Final rotation to align with the final orientation
		float theta2_ = 0;

		//Tuning parameters
		params params_;


	};


}//end namespace str