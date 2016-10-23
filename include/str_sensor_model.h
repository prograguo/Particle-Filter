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