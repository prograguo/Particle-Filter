#include "str_sensor_model.h"


namespace str
{
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
		normalFunction[i] = 1.0 / pow(2.0*sigSq*M_PI, 0.5) *
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

}//namespace str