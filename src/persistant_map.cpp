#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <unistd.h> // usleep

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>

#include "gnuplot-iostream.h"
#include "types.h"
#include "str_io.h"
#include "str_sensor_model.h"
#include "helper_functions.h"

#include "bee-map.h"
#include "bee-map-impl.h"



int main() {
	Gnuplot gp;
	//gp << "set zrange [-1:1]\n";
	//gp << "set hidden3d nooffset\n";

	map_type costMap;
	char datLoc[] = "data/map/wean.dat";
	int val = read_beesoft_map(datLoc, &costMap);
	std::vector<std::pair<float, float>> plotMap;

	for(int i = 0; i < costMap.size_x; i++)
	{
		for(int j = 0; j < costMap.size_y; j++)
		{
			//std::cout << costMap.prob[i][j] << " ";

			// CostMap.prob has 
			// 0 probability for free space
			// >0 prob for occupied voxels
			// <0 for unexplored
			if(costMap.prob[i][j] > 0){
				float x = i;
				float y = j;
				plotMap.push_back(std::pair<float,float>(x,y));
			}
		}
		std::cout << "\n";
	}


	std::vector<str::laser> laserData;
	std::vector<str::odom> odomData;
	str::readRobotData("data/log/ascii-robotdata2.log", laserData,	odomData);
	std::vector<std::pair<float, float>> plotOdom;

	gp << "set multiplot layout 1,2\n";
	gp << "set origin 0,0\n";
	gp << "set size 1,1\n";
	gp << "plot '-' title 'prob'\n";
	gp.send1d(plotMap);
	for( auto it = odomData.begin(); it!= odomData.end(); it++)
	{
		plotOdom.clear();
		plotOdom.push_back(std::pair<float, float>(it->x_cm, it->y_cm));
		gp << "set origin 0,0\n";
		gp << "set size 1,1\n";
		gp << "plot '-' title 'odom' pt 7 ps 3\n";
		gp.send1d(plotOdom);
		gp << "unset multiplot\n";
	}


	return 0;
}
