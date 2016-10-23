// Class that contains the particle filter 
// Needs the map to be initialized and is updated every time an update comes from a log 


#include "particle_filter.h"


namespace str
{

particle_filter::particle_filter(libconfig::Config &cfg)
{	
	motion_model_=std::make_shared<motion_model>(motion_model(cfg));
}

void particle_filter::filter_update(odom odometry_reading, laser laser_reading)
{

	particles new_particles;

	//Update the motion model
	motion_model_->update_odometry(odometry_reading);
	motion_model_->propagate_particles(particle_set_);


	for (size_t p_idx=0; p_idx < particle_set_.size(); ++p_idx)
	{
		//Update weight of particle based on sensor model
	}

	//Resample the particles based on their updated weights
	resample(new_particles);

	//Copy these new particles to your particle set
	// particle_set_.clear();
	// particle_set_.copy(new_particles.begin(), new_particles.end());
}

void particle_filter::resample(particles& new_particles)
{
	//Make sure new particles are empty
	new_particles.clear();	

	
	unsigned int num_draws = particle_set_.size();
	float num_draws_inv = 1.0/num_draws;

	// Generate a random number between 0 and num_draws_inv
	float random_number = (std::rand()/RAND_MAX)*num_draws_inv;

	//First Weight 
	float w  = particle_set_.front().weight;

	// Upper bound for resampling
	float upper_bound = 0;

	unsigned int i= 0;

	for (size_t particle_idx = 0 ; particle_idx < particle_set_.size(); ++particle_idx)
	{
		upper_bound = random_number + (particle_idx+1)*num_draws_inv;
		
		while(upper_bound>w)
		{
			++i;
			w+=particle_set_.at(i).weight;
		}
		new_particles.push_back(particle_set_.at(i));
	}
}

}//end namespace str