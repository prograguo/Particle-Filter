#ifndef __str_sensor_model__
#define __str_sensor_model__

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <cmath>

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
	double rangeSTD)
{
	std::vector<double> sensorModel(MAX_RANGE);
	std::vector<double> decayFunction(MAX_RANGE);
	std::vector<double> maxFunction(MAX_RANGE);
	std::vector<double> normalFunction(MAX_RANGE);

	double decaySum = 0;
	double maxSum = maxParam;
	double normalSum = 0;
	double uniformSum = uniformParam*MAX_RANGE;

	// construct each of the function individually
	for(int i = 0; i < MAX_RANGE; i++)
	{
		decayFunction[i] = decayScale*exp(-decayRate*double(i));
		decaySum += decayFunction[i];

		double sigSq = pow(rangeSTD, 2);
		normalFunction[i] = 1.0 / pow(2.0*sigSq*M_PI, -0.5) *
			exp(-1.0* pow(double(i) - rangeMean, 2)/ (2.0*sigSq));
		normalSum += normalFunction[i];
	}
	maxFunction[MAX_RANGE-1] = maxParam;

	// combine each of the functions in to the sensorModel
	sensorModel.clear();
	sensorModel.reserve(MAX_RANGE);
	for(int i = 0; i < MAX_RANGE; i++)
	{
		 sensorModel[i] = (
			decayFunction[i] + maxFunction[i] + 
			normalFunction[i] + uniformParam ) 
			/ (decaySum + maxParam + normalSum + uniformParam);

	}
	return sensorModel;
}

} // ns str
#endif