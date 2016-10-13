#ifndef __str_io__
#define __str_io__

#include <vector>
#include <iostream>
#include <fstream>
#include <string>

namespace str{
	/*
	Entry Type #1 (odometry):

	O x y theta ts

	x y theta - coordinates of the robot in standard odometry frame
	ts - timestamp of odometry reading (0 at start of run)
	*/
	struct odom{
		double x;
		double y;
		double theta;
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
		std::vector<float> r;
		double ts;
	};

	/* 
	Function to read robot data from file
	*/
	void readRobotData(
		std::string data_path, 
		std::vector<laser>& laserData, 
		std::vector<odom>& odomData )
	{
		std::ifstream file;
		std::string line;
		char controlFlag;
		double x, y, theta, ts;

		file.open(data_path);
		while (std::getline(file, line))
		{
			std::istringstream ss(line);
			ss >> controlFlag;
			if(controlFlag == 'L'){
				// handle laser data
				//TODO
			}else{
				//handle odom data
				ss >> x >> y >> theta >> ts;
				odom oData;
				oData.x = x;
				oData.y = y;
				oData.theta = theta;
				oData.ts = ts;
				odomData.push_back(oData);
			}
		}


	}










} // ns str
#endif