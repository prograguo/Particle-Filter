#ifndef __MOTION_MODEL__
#define __MOTION_MODEL__

#include <libconfig.h++>

#include "particle.h"
#include "types.h"
#include "helper_functions.h"

namespace str
{

// Class that takes is initialized with an initial estimate of odometry
// and is used to propagate any particle.
	class motion_model
	{
	public:

		motion_model(libconfig::Config &cfg);

		void propagate_particles(particles& init_particles);

		void update_odometry(const odom& reading);

	private:

		// Current Odometry Reading
		odom current_reading_;

		//Perturbation for the first rotation to align with the translation axis
		float theta1_rad = 0;

		//Perturbation for the translation along the translation axis
		float translation_cm = 0;

		//Perturbation for the final rotation to align with the final orientation
		float theta2_rad = 0;

		//Tuning parameters
		float alpha1_;
		float alpha2_;
		float alpha3_;
		float alpha4_;



	};


}//end namespace str


#endif