#include <vector>
#include <iostream>
#include <cmath>
#include <string>

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>

#include "gnuplot-iostream.h"
#include "str_io.h"

int main() {
	Gnuplot gp;
	//gp << "set zrange [-1:1]\n";
	//gp << "set hidden3d nooffset\n";

	std::vector<str::laser> laserData;
	std::vector<str::odom> odomData;
	str::readRobotData("data/log/robotdata1.log", laserData,	odomData);

	for( auto it = odomData.begin(); it!= odomData.end(); it++)
	{
		std::cout << it->x << "\n";
	}


	return 0;
}
