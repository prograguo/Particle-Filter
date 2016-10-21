// Class that contains the particle filter 
// Needs the map to be initialized and is updated every time an update comes from a log 


#ifndef __PARTICLE_FILTER__
#define __PARTICLE_FILTER__

class particle_filter
{
public:
	void filter_update(odom odometry_reading, laser laser_reading);

private:

	//Low variance importance resampling of particles 
	void resample(particles& new_particles);
	
	unsigned int number_of_particles;

	motion_model m_model_;

	sensor_model s_model_;

	particles particle_set_;

	map_type map_;

};