#ifndef __str__types__
#define __str__types__

#include <vector>

namespace str
{

	const int MAX_RANGE = 8183; // empirically determined

	/*
	Entry Type #1 (odometry):
	O x y theta ts
	x y theta - coordinates of the robot in standard odometry frame
	ts - timestamp of odometry reading (0 at start of run)
	*/
	struct odom{
		double x_cm;
		double y_cm;
		double theta_deg;
		double ts;
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

} // ns str

#endif