//Definition of helper functions used for the filter
#include <random>

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
		srand (time(NULL));
		float sum=0;
		for (size_t i=0;i<12;++i)
		{
			sum+= (2*double(std::rand())/RAND_MAX)-1;
		}
		return (variance/6.0)*sum;
	}

}