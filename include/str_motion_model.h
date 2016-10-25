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

		motion_model(libconfig::Config &cfg, odom& initial);

		void propagate_particles(particles& init_particles);

		void update_odometry( odom& reading);

	private:

		// Current Odometry Reading
		odom current_reading_;

		//Perturbation for the first rotation to align with the translation axis
		double theta1_rad_ = 0;

		//Perturbation for the translation along the translation axis
		double translation_cm_ = 0;

		//Perturbation for the final rotation to align with the final orientation
		double theta2_rad_ = 0;

		//Tuning parameters
		float alpha1_;
		float alpha2_;
		float alpha3_;
		float alpha4_;



	};


}//end namespace str


#endif