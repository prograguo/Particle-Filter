#ifndef __str_io__
#define __str_io__

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "types.h"

namespace str{

	namespace
	{
		const int MAP_SIZE = 4;

	}

	void readRobotData(
		std::string data_path, 
		std::vector<laser>& laserData, 
		std::vector<odom>& odomData );


	void readMap(
		std::vector<std::vector<float>>& grid, 
		std::string filename);

} // ns str
#endif