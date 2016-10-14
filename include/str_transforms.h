#ifndef __str_transforms__
#define __str_transforms__

#include <vector>
#include <cmath>

namespace str{

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
	}


} // ns str
#endif