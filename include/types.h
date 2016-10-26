#ifndef __str__types__
#define __str__types__

#include <vector>
#include <map>
#include <unordered_map>

namespace str
{

	const int MAX_RANGE = 8192; // empirically determined

	/*
	Entry Type #1 (odometry):
	O x y theta ts
	x y theta - coordinates of the robot in standard odometry frame
	ts - timestamp of odometry reading (0 at start of run)
	*/
	struct odom{
		double x_cm;
		double y_cm;
		double theta_rad;
		double ts;

		odom& operator=(odom& other)
    {
        x_cm= other.x_cm;
        y_cm= other.y_cm;
        theta_rad= other.theta_rad;
        ts= other.ts;
        return *this;
    }

	};

	/*
	Entry Type #2 (laser)

	L x y theta xl yl thetal r1 ... r180 ts
	x y theta - coodinates of the robot in standard odometry frame when
	laser reading was taken (interpolated)
	xl yl thetal - coordinates of the *laser* in standard odometry frame
	when the laser reading was taken (interpolated)
	r1 .. r180 - 180 range readings of laser in cm.  The 180 readings span
	180 degrees *STARTING FROM THE RIGHT AND GOING LEFT*  Just like angles,
	the laser readings are in counterclockwise order.
	ts - timestamp of laser reading
	*/
	struct laser{
		double x;
		double y;
		double theta;
		double xl;
		double yl;
		double thetal;
		std::vector<int> r;
		double ts;
	};

	// Parameters used in the sensor model
	struct sensor_model_params {
		double uniformParam;
		double decayScale;
		double decayRate;
		double maxParam;
		double rangeSTD;
		double gaussianGain;
	};

	 // Simple hash for pair of integers
    struct SimpleHash {
        size_t operator()(const std::pair<int, int>& p) const {
            return p.first ^ p.second;
        }
    };
    
    typedef std::unordered_map<std::pair<int, int>, std::vector<int>, SimpleHash> range_map;
   	

   	//
    // Trigonometric data required for each angle in degrees
    //
    struct angle_data {
        double radians;
        double tmaxX;
        double tmaxY;
        double tDeltaX;
        double tDeltaY;
    };


} // ns str

#endif