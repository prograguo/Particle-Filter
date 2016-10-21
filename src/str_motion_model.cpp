#include "str_motion_model.h"
#include <cmath>

namespace str
{

motion_model::motion_model(params parameters)
{
	params_ = parameters;
}

void motion_model::update_odometry(const odom& reading1, const odom2& reading2)
{

	float theta1_rad = std::atan2((reading2.y_cm-reading1.y_cm),(reading2.x-reading1.x)) - angle_degree_to_radians(reading1.theta_deg); 

	float trans_cm = std::hypot((reading2.y-reading1.y),(reading2.x-reading1.x));

	float theta2_rad = angle_degree_to_radians(reading2.theta_deg - reading1.theta_deg);

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
	theta1_rad = theta1_rad - sample_from_gaussian(params_.alpha1*theta1_rad + params_.alpha2*trans_cm);

	translation_rad = trans_cm - sample_from_gaussian(params_.alpha3*trans_cm + params_.alpha4*(theta1_rad+theta2_rad));   

	theta2_rad = theta2_rad - sample_from_gaussian(params_.alpha1*theta2_rad + params_.alpha2*trans_cm);
}

void motion_model::propagate_particle(particle& particle)
{
	particle.x_cm+= translation_cm* std::cos(partcle.theta_deg + theta1_rad);
	particle.y_cm+= translation_cm* std::sin(partcle.theta_deg + theta1_rad);
	particle.theta_deg+=  angle_radians_to_degree(theta1_rad+theta2_rad);
}

}//end namespace str