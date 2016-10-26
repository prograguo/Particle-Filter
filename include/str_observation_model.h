#ifndef __str_observation_model__
#define __str_observation_model__


#include <vector>
#include <cmath>

#include "str_ray_tracer.h"
#include "bee-map.h"
#include "types.h"
#include "grapher.h"
#include "str_sensor_model.h"

#define MAX_DEGREES 360
#define SENSOR_FOV 180

namespace str {
    class observation_model {
        
        RayTracer ray_tracer;
        SensorModel sensor_model;

    public:
        observation_model(sensor_model_params sensorParams);

        void forcePopulateRangeCache(map_type &map);

        double getProbForParticle(particle &p, laser &l, 
        	map_type &map, str::Grapher &grapher, int disablePlotting);

        void getSensorModel(std::vector<double>& vals);
    };
}
#endif