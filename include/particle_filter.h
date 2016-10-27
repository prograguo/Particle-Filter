// Class that contains the particle filter 
// Needs the map to be initialized and is updated every time an update comes from a log 


#ifndef __PARTICLE_FILTER__
#define __PARTICLE_FILTER__

#include "str_io.h"
#include "helper_functions.h"
#include "particle.h"
#include "str_motion_model.h"
#include "str_sensor_model.h"
#include "bee-map.h"
#include "str_observation_model.h"
#include "grapher.h"

#include <cstdlib>
#include <memory>
#include <libconfig.h++>
#include <functional>
#include <memory>


namespace str
{
class particle_filter
{
public:
	particle_filter(libconfig::Config &cfg,odom &inital_odom, 
		const map_type& map,
		Grapher &grapher);
	void filter_update_odom(odom &odometry_reading);
	void filter_update_laser(laser &laser_reading, int enableSensorPlotting);
	void generate_random_particles();
	void get_particle_set(particles& p){ p = particle_set_;};
	particle get_centroid();


private:


	//Low variance importance resampling of particles 
	void resample(particles& new_particles);
	
	unsigned int number_of_particles;

	Grapher& grapher_;

	std::shared_ptr<motion_model> motion_model_;

	std::shared_ptr<observation_model> observation_model_;

	particles particle_set_;

	map_type map_;

	sensor_model_params sensor_params_;

	int n_particles_;

	double sum_of_particles_;


};

}// end namespace str

#endif