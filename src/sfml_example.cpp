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
#include "str_motion_model.h"

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
		particleSet.push_back(str::particle(400,0,0));
		// std::pair<int, int> pt;
		// int r_pt = (std::rand() * freeSpace.size()) / RAND_MAX ;
		// pt = freeSpace[r_pt];
		// str::particle newParticle(pt.first, pt.second, 0);
		// particleSet.push_back(newParticle);
	}

	str::Grapher grapher(width, width);
	grapher.setMap(costMap.prob);

	libconfig::Config cfg;
	try{
		cfg.readFile("config/params.cfg");
	}catch(const libconfig::FileIOException &fioex){
		std::cerr << "I/O error while reading file\n";
		return 0
		;	}catch(const libconfig::ParseException &pex){
			std::cerr << "Parse error at " << pex.getFile() <<
			":" << pex.getLine() << " - " << pex.getError() << "\n";
			return 0;
		}

		str::odom initial={0,0,0,0};

		str::motion_model m_model(cfg,initial);


		// for(int i = 0; i < laserData.size(); i++)
		// {	
		// 	grapher.setParticlePoints(particleSet);
		// 	grapher.setLaserLines(laserData[i].r, 300, 300);
		// 	grapher.updateGraphics();
		// }

		for (size_t i=0;i<1000;++i)
		{
			m_model.update_odometry(initial);
			m_model.propagate_particles(particleSet);	
			grapher.setParticlePoints(particleSet);
			grapher.updateGraphics();

		}	

		return 0;
	}
