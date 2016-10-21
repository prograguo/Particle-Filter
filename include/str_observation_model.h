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

        double getProbForParticle(particle &p, laser &l, map_type &map)
        {
            std::vector<double> expected_ranges;
            ray_tracer.getRangesFromPoint(map, p, expected_ranges);
            std::cout<<"Got ranges\n";
            double logObservationProb = 0;

            // Calculate the starting angle of the laser reading for 0-360 values, which is p.theta_deg - 90
            // And ensure its between 0 and 359
            int angleIndex = int(round(p.theta_deg - 90 + MAX_DEGREES)) % MAX_DEGREES;
            std::cout<<"Got start angle index "<<angleIndex<<'\n';
            for(int i=0; i<SENSOR_FOV; ++i)
            {
                // Get the range value for that angle.
                // Note: Approximation is done to the range at nearest degree.
                int rangeMean = round(expected_ranges[angleIndex]);
                int rangeObs = l.r[i];
                std::cout<<"Range mean: "<<rangeMean<<". Range observed: "<<rangeObs<<'\n';
                double prob = sensor_model.getObservationProbability(rangeMean, rangeObs);
                logObservationProb += log(prob);

                // Update the angle index. Might need to rotate back to front
                angleIndex = angleIndex + 1;
                if(angleIndex > MAX_DEGREES)
                    angleIndex = angleIndex - MAX_DEGREES;
            }
            return logObservationProb;
        }

    };
}