#include "str_observation_model.h"

namespace str
{

        observation_model::observation_model(sensor_model_params sensorParams) : sensor_model(SensorModel(sensorParams))
        {}

        void observation_model::forcePopulateRangeCache(map_type &map)
        {
            std::cout<<"Populating range cache..\n";
            ray_tracer.populateRangeCache(map);
        }

        double observation_model::getProbForParticle(particle &p, laser &l, map_type &map, str::Grapher &grapher)
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
            int angleIndex = int(round(p.theta_rad / M_PI*180.0 - 90 + MAX_DEGREES)) % MAX_DEGREES;
            // std::cout<<"Got start angle index "<<angleIndex<<" Laser range data size: "<<l.r.size()<<'\n';
    
            // TODO(Tushar) : Only used for graphing
            std::vector<int> expected_ranges_tmp(l.r.size(), -1);

            for(int i=0; i< l.r.size(); ++i)
            {
                // Get the range value for that angle.
                // Note: Approximation is done to the range at nearest degree.
                int rangeMean = expected_ranges[angleIndex];
                int rangeObs = l.r[i];
                // std::cout<<"Index: "<<i<<". Angle index in abs: "<<angleIndex<<". Range mean: "<<rangeMean<<". Range observed: "<<rangeObs<<'\n';
                
                double prob = sensor_model.getObservationProbability(rangeMean, rangeObs);
                // std::cout<<"Prob: "<<prob;
                logObservationProb += log(prob);

                // Update the angle index. Might need to rotate back to front
                angleIndex = angleIndex + 1;
                if(angleIndex >= MAX_DEGREES)
                    angleIndex = angleIndex - MAX_DEGREES;
                
                // TODO(Tushar) : Only used for graphing    
                expected_ranges_tmp[i] = rangeMean;
            }
            
            // TODO(Tushar) : Only used for graphing
            grapher.setMeasuredRanges(l.r);
            grapher.setPredictedRanges(expected_ranges_tmp);
            grapher.updateSensorGraphics();
            
          	p.weight = logObservationProb;
            return logObservationProb;
        }


}//end namespace str