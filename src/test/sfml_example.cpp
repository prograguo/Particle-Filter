#include <vector>
#include <iostream>
#include <cmath>
#include <string>
#include <cstdlib> // ex-handling, rand
#include <unistd.h> // usleep

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>

#include <libconfig.h++>


#include "types.h"
#include "str_io.h"
#include "str_sensor_model.h"
#include "helper_functions.h"
#include "grapher.h"
#include "particle.h"

#include "bee-map.h"
#include "bee-map-impl.h"

int main() {

	std::vector<str::laser> laserData;
	std::vector<str::odom> odomData;
	str::readRobotData("data/log/ascii-robotdata2.log", laserData,	odomData);
	
	map_type costMap;
	std::vector<std::pair<int, int>> freeSpace;
	char datLoc[] = "data/map/wean.dat";
	int val = read_beesoft_map(datLoc, &costMap);

	unsigned int width = costMap.size_x;
	unsigned int height = costMap.size_y;

	// populate the vertex array, with one quad per tile
	for (unsigned int i = 0; i < width; ++i)
	{
	  for (unsigned int j = 0; j < height; ++j)
	  {
	  	if(costMap.prob[i][j] == 0){
	  		freeSpace.push_back(std::pair<int, int>(i,j));
	  	}
	  }
	}

	int N_Particles = 1000;
	std::vector<str::particle> particleSet;
	for (unsigned int i = 0; i < N_Particles; i++)
	{
		std::pair<int, int> pt;
		int r_pt = (std::rand() * freeSpace.size()) / RAND_MAX ;
		pt = freeSpace[r_pt];
		str::particle newParticle(pt.first, pt.second, 0);
		particleSet.push_back(newParticle);
	}

	str::Grapher grapher(width, width);
	grapher.setMap(costMap.prob);

	for(int i = 0; i < laserData.size(); i++)
	{
		grapher.setParticlePoints(particleSet);
		grapher.setLaserLines(laserData[i].r, 300, 300);
		grapher.updateGraphics();
	}



 



	return 0;
}
