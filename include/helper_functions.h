#ifndef __str_helper_functions__
#define __str_helper_functions__

// File that contains helper functions, useful accross all files

#include <vector>
#include <cmath>

namespace str
{

//Checks angles are between [-pi,pi]
bool check_angle(const double &angle);

void trim_angle_radians(double &angle);

double angle_radians_to_degree(const double angle_rad);

double angle_degree_to_radians(const double angle_deg);

double sample_from_gaussian(double variance);

double sample_from_gaussian(double mean, double variance);

float sample_from_gaussian2(float mean, float variance);

std::vector<std::pair<double,double>>
		range2Point(const std::vector<int>& ranges);

} // sn str

#endif