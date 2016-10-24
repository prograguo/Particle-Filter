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
				gaussianFunction[i] = 1.0 / pow(2.0*sigSq*M_PI, -0.5) *
					exp(-1.0* pow(double(i), 2)/ (2.0*sigSq));
			}
			maxFunction = sensorParams.maxParam;
			uniformParam = sensorParams.uniformParam;
		}

		double getObservationProbability(int rangeMean, int rangeObserved)
		{
			if(rangeMean < 0 || rangeMean > MAX_RANGE)
				std::cout<<"ERROR: Range mean from ray tracer is invalid: "<<rangeMean<<". Crashing..\n";
			double result;
			if(rangeMean == MAX_RANGE-1)
				result += maxFunction;
			result += expDecayFunction[rangeMean] + uniformParam + gaussianFunction[abs(rangeObserved - rangeMean)];
			return result;
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