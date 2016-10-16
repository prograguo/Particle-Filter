#ifndef __helper_functions__
#define __helper_functions__

// File that contains helper functions, useful accross all files

#include <vector>
#include <cmath>

namespace str
{

//Checks angles are between [-pi,pi]
bool check_angle(const float &angle);

void trim_angle_radians(float &angle);

float angle_radians_to_degree(const float angle_rad);

float angle_degree_to_radians(const float angle_deg);

float sample_from_gaussian(float variance);

std::vector<std::pair<double,double>>
range2Point(std::vector<int>& ranges){
	std::vector<std::pair<double,double>> xy;
	for(int i = 0 ; i < ranges.size(); i++)
	{
		double rad = double(i)*M_PI/180.0;
		std::pair<double, double> pt(cos(rad)*ranges[i], sin(rad)*ranges[i]);
		xy.push_back(pt);
	}
	return xy;
};

} // sn str

#endif