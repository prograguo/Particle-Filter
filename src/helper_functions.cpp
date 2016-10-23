//Definition of helper functions used for the filter
#include <random>
#include <chrono>
#include "helper_functions.h"


namespace str
{
	namespace
	{
		const double PI =3.141592653;
		// std::default_random_engine e;
		// std::gaussian_int_distribution<> d();
		// std::function<float<>> gaussian_sampling = std::binf(d,e);
	}
	bool check_angle(const float &angle)
	{
		return (angle>-PI && angle <PI);
	}

	void trim_angle_radians(float &angle)
	{	
		int sign = angle >=0 ? 1:-1;

		while(!check_angle(angle))
		{
			angle-= sign*PI;
		}
	}

	float angle_radians_to_degree(const float angle_rad)
	{
		return (angle_rad*180)/PI;
	}

	float angle_degree_to_radians(const float angle_deg)
	{
		return (angle_deg*PI)/180;
	}


	float sample_from_gaussian(float variance)
	{	
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
		std::default_random_engine generator(seed);
		std::normal_distribution<double> distribution(0.0,variance);
		return distribution(generator);
		/*
		srand (time(NULL));
		float sum=0;
		for (size_t i=0;i<12;++i)
		{
			sum+= (2*double(std::rand())/RAND_MAX)-1;
		}
		return (variance/6.0)*sum;
		*/
	}

	std::vector<std::pair<double,double>>
		range2Point(const std::vector<int>& ranges)
	{
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
