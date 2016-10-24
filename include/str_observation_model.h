#ifndef __str_observation_model__
#define __str_observation_model__
#endif

#include <vector>
#include <cmath>

#include "str_ray_tracer.h"
#include "bee-map.h"
#include "types.h"

#define MAX_DEGREES 360
#define SENSOR_FOV 180

namespace str {
    class observation_model {
        
        RayTracer ray_tracer;
        SensorModel sensor_model;

    public:
        observation_model(sensor_model_params sensorParams) : sensor_model(SensorModel(sensorParams))
        {}

        void forcePopulateRangeCache(map_type &map)
        {
            std::cout<<"Populating range cache..\n";
            ray_tracer.populateRangeCache(map);
        }

        double getProbForParticle(particle &p, laser &l, map_type &map)
        {
            // Get ranges for that particle - full 360 degrees
            std::vector<int> expected_ranges(MAX_DEGREES, -1);
            ray_tracer.getRangesFromPoint(map, p, expected_ranges);
            
            // // Debug Prints
            // std::cout<<"Got ranges "<<expected_ranges.size()<<'\n';
            // for( auto i=0; i<expected_ranges.size(); ++i)
            //     std::cout<<i<<" : "<<expected_ranges[i]<<'\n';
            
            double logObservationProb = 0;

            // Calculate the starting angle of the laser reading for 0-360 values, which is p.theta_deg - 90
            // And ensure its between 0 and 359
            int angleIndex = int(round(p.theta_deg - 90 + MAX_DEGREES)) % MAX_DEGREES;
            // std::cout<<"Got start angle index "<<angleIndex<<" Laser range data size: "<<l.r.size()<<'\n';
    
            for(int i=0; i< l.r.size(); ++i)
            {
                // Get the range value for that angle.
                // Note: Approximation is done to the range at nearest degree.
                int rangeMean = expected_ranges[angleIndex];
                int rangeObs = l.r[i];
                // std::cout<<"Index: "<<i<<". Angle index in abs: "<<angleIndex<<". Range mean: "<<rangeMean<<". Range observed: "<<rangeObs<<'\n';
                
                double prob = sensor_model.getObservationProbability(rangeMean, rangeObs);
                logObservationProb += log(prob);

                // Update the angle index. Might need to rotate back to front
                angleIndex = angleIndex + 1;
                if(angleIndex >= MAX_DEGREES)
                    angleIndex = angleIndex - MAX_DEGREES;
            }
            return logObservationProb;
        }

    };
}