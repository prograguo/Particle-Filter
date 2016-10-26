#ifndef __str_sensor_model__
#define __str_sensor_model__

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>

#include "types.h"

namespace str{

	class SensorModel
	{
		double expDecayFunction[MAX_RANGE];
		double gaussianFunction[MAX_RANGE];
		double maxFunction;
		double uniformParam;
	public:
		SensorModel(sensor_model_params sensorParams)
		{
			// construct each of the function individually
			for(int i = 0; i < MAX_RANGE; i++)
			{
				expDecayFunction[i] = sensorParams.decayScale*exp(-sensorParams.decayRate*double(i));
				double sigSq = pow(sensorParams.rangeSTD, 2);
				gaussianFunction[i] = sensorParams.gaussianGain / pow(2.0*sigSq*M_PI, 0.5) *
					exp(-1.0* pow(double(i), 2)/ (2.0*sigSq));
				// std::cout<<gaussianFunction[i]<<'\t';
			}
			maxFunction = sensorParams.maxParam;
			uniformParam = sensorParams.uniformParam;
		}

		double getObservationProbability(int rangeMean, int rangeObserved)
		{
			if(rangeMean < 0 || rangeMean > MAX_RANGE)
				std::cout<<"ERROR: Range mean from ray tracer is invalid: "<<rangeMean<<". Crashing..\n";
			double result = 0;
			if(rangeMean == MAX_RANGE) // Do not consider in prob calculation
				return -1;
			if(rangeMean == MAX_RANGE-1)
				result += maxFunction;
			result += (expDecayFunction[rangeObserved] + uniformParam + gaussianFunction[abs(rangeObserved - rangeMean)])/3;
			return result;
		}

		void getSensorModel(std::vector<double>& vals)
		{
			vals.clear(); 
			vals.reserve(MAX_RANGE);
			for(int i = 0; i < MAX_RANGE; i++)
			{
				vals.push_back(getObservationProbability( 4000, i));
			}
			vals[MAX_RANGE-1] += maxFunction;
		}

	};

std::vector<double>
 generateSensorModel(
	double uniformParam,
	double decayScale,
	double decayRate,
	double maxParam,
	double rangeMean,
	double rangeSTD);

} // ns str
#endif