#include "str_motion_model.h"

#include <cmath>
#include <libconfig.h++>

namespace str
{

	motion_model::motion_model(libconfig::Config &cfg)
	{
		alpha1_ = cfg.lookup("motionModel.alpha1");		
		alpha2_ = cfg.lookup("motionModel.alpha2");		
		alpha3_ = cfg.lookup("motionModel.alpha3");		
		alpha4_ = cfg.lookup("motionModel.alpha4");		
	}


	void motion_model::propagate_particles(particles& init_particles)
	{
		for (auto& curr_particle:init_particles)
		{
		curr_particle.x_cm+= translation_cm* std::cos(curr_particle.theta_deg + theta1_rad);
		curr_particle.y_cm+= translation_cm* std::sin(curr_particle.theta_deg + theta1_rad);
		curr_particle.theta_deg+=  angle_radians_to_degree(theta1_rad+theta2_rad);
		}
	}

	void motion_model::update_odometry(const odom& reading)
	{

		float theta1_rad = std::atan2((reading.y_cm-current_reading_.y_cm),(reading.x_cm-current_reading_.x_cm)) 
		                      - angle_degree_to_radians(current_reading_.theta_deg); 

		float trans_cm = std::hypot((reading.y_cm-current_reading_.y_cm),(reading.x_cm-current_reading_.x_cm));

		float theta2_rad = angle_degree_to_radians(reading.theta_deg - current_reading_.theta_deg);

	// Trim the calculated angles if they are not between [-PI,PI]
		if(!check_angle(theta1_rad))
		{
			trim_angle_radians(theta1_rad);
		}

		if(!check_angle(theta2_rad))
		{
			trim_angle_radians(theta2_rad);
		}

	//Calculate normally distributed perturbations
		theta1_rad = theta1_rad - sample_from_gaussian(alpha1_*theta1_rad + alpha2_*trans_cm);

		translation_cm = trans_cm - sample_from_gaussian(alpha3_*trans_cm + alpha4_*(theta1_rad+theta2_rad));   

		theta2_rad = theta2_rad - sample_from_gaussian(alpha1_*theta2_rad + alpha2_*trans_cm);
	
	//Update the final odometry value
		current_reading_ = reading;
	}

}//end namespace str