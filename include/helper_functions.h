#ifndef __str_helper_functions__
#define __str_helper_functions__

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

float sample_from_gaussian2(float mean, float variance);

std::vector<std::pair<double,double>>
		range2Point(const std::vector<int>& ranges);

} // sn str

#endif