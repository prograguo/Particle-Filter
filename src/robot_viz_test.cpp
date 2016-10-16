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



	gp << "set term x11 3\n";
	gp << "plot '-' title 'prob'\n";
	gp.send1d(plotMap);

	char c;
	std::cout <<"any key to continue...\n";
	std::cin >> c;

	std::vector<str::laser> laserData;
	std::vector<str::odom> odomData;
	str::readRobotData("data/log/ascii-robotdata2.log", laserData,	odomData);
	gp << "set term x11 0\n";
	for(int j = 0; j < str::MAX_RANGE; j+= 250){
		std::vector<double> sensorModel;
		sensorModel = str::generateSensorModel( 
			0.0001, 1000, 0.0001, 1000, j, 250);


		
		std::vector<std::pair<int, double>> sensorModelPlot(str::MAX_RANGE);
		
		for(int i = 0; i < str::MAX_RANGE; i++){

			std::pair<int,double> pt(i, sensorModel[i]);
			sensorModelPlot[i] = pt;
		}



		gp << "plot '-' title 'pts_A'\n";
		gp.send1d(sensorModelPlot);
		
	}


	gp << "set term x11 1\n";
	for( auto it = odomData.begin(); it!= odomData.end(); it++)
	{
		;
	}

	float maxVal = 0;
	for( auto it = laserData.begin(); it!= laserData.end(); it++)
	{

		for(auto it2 = it->r.begin(); it2 != it->r.end(); it2++)
		{
			if( *it2 > maxVal){
				maxVal = *it2;
				std::cout << "MaxValue is " << maxVal << "\n";
				
			}
		}

		std::vector<std::pair<double,double>> xy;
		xy = str::range2Point(it->r);
		gp << "set xrange [-1000:1000]\nset yrange [-200:1250]\n";
		gp << "plot '-' title 'pts_A'\n";
		gp.send1d(xy);
		usleep(10);

	}




	//plot again

	gp << "set term x11 0\n";
	gp << "set autoscale\n";
	for(int j = 0; j < str::MAX_RANGE; j+= 250){
		std::vector<double> sensorModel;
		sensorModel = str::generateSensorModel( 
			0.0001, 1000, 0.0001, 1000, j, 250);


		
		std::vector<std::pair<int, double>> sensorModelPlot(str::MAX_RANGE);
		
		for(int i = 0; i < str::MAX_RANGE; i++){

			std::pair<int,double> pt(i, sensorModel[i]);
			sensorModelPlot[i] = pt;
		}



		gp << "plot '-' title 'pts_A'\n";
		gp.send1d(sensorModelPlot);
		
	}




	return 0;
}
