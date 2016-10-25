//Definition of helper functions used for the filter
#include <random>
#include "helper_functions.h"


namespace str
{
	namespace
	{
		const double PI =3.141592653;
		
		std::mt19937 mt(1729);
		// std::default_random_engine e;
		// std::gaussian_int_distribution<> d();
		// std::function<float<>> gaussian_sampling = std::binf(d,e);
	}
	bool check_angle(const double &angle)
	{
		return (angle>-PI && angle <PI);
	}

	void trim_angle_radians(double &angle)
	{	
		int sign = angle >=0 ? 1:-1;

		while(!check_angle(angle))
		{
			angle-= sign*PI;
		}
	}

	double angle_radians_to_degree(const double angle_rad)
	{
		return (angle_rad*180)/PI;
	}

	double angle_degree_to_radians(const double angle_deg)
	{
		return (angle_deg*PI)/180;
	}

	//Approximate Gaussian Sampling
	double sample_from_gaussian(double variance)
	{	
		srand (time(NULL));
		float sum=0;
		for (size_t i=0;i<12;++i)
		{
			sum+= (2*double(std::rand())/RAND_MAX)-1;
		}
		return (variance/6.0)*sum;
	}

	//Gaussian sampling from more accurate distribution
	double sample_from_gaussian(double mean, double variance)
	{
		std::normal_distribution<float> gauss(mean,variance);
		return gauss(mt);
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

	int getQuadrant(double angle)
    {
        return angle/90;        
    }

    bool isObstacle(double value)
    {
        return value > 0.5;
    }

     std::pair<int,int> cmToMapCoordinates(int x_cm, int y_cm, int map_res)
    {
        return std::pair<int,int>(x_cm/map_res, y_cm/map_res);
    }

    double mapToCmCoordinates(double val, int map_res)
    {
        return val * map_res;
    }

    double sample_from_uniform(const double min, const double max)
    {
		std::uniform_real_distribution<> dis(min, max);
		return dis(mt);
    }

    double normalize_weights(particles& particle_set)
    {	
    	auto max = std::max_element(particle_set.begin(), particle_set.end(), [&] (const particle &particle1,const particle &particle2)
    																					{
    																						return (particle1.weight<particle2.weight);
    																					});
    	for (auto& particle:particle_set)
    	{
    		particle.weight/= max->weight;
    	}
    }

} // ns str
