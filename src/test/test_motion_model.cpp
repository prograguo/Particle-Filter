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

void printOdom(const str::odom& data)
{
	std::cout << "odom " << 
		"x: " << data.x_cm << 
		", y: " << data.y_cm <<
		", Th: " << data.theta_deg << 
		", Time: " << data.ts << "\n";
}

int main() {

	std::vector<str::laser> laserData;
	std::vector<str::odom> odomData;
	str::readRobotData("data/log/manual.log", laserData,	odomData);
	
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

	// int N_Particles = 1;
	int N_Particles = 10;
	str::particles particleSet;
	for (unsigned int i = 0; i < N_Particles; i++)
	{
		//std::pair<int, int> pt;
		//int r_pt = (std::rand() * freeSpace.size()) / RAND_MAX ;
		//pt = freeSpace[r_pt];

		// for (auto &o:odomData)
		// {
		// str::particle newParticle(o.x_cm,o.y_cm, 0);
		str::particle newParticle(400,400, 0);
		particleSet.push_back(newParticle);
		// }
	}

	str::Grapher grapher(width, width);
	grapher.setMap(costMap.prob);

	libconfig::Config cfg;
	cfg.readFile("config/params.cfg");

	str::odom initial=odomData[0];
	str::motion_model motionModel(cfg,initial);

	for(int i = 0; i < odomData.size()-5; i++)
	{
		std::cout << "iteration: " << i << "\n";
		printOdom(odomData[i]);
		motionModel.update_odometry(odomData[i]);
		motionModel.propagate_particles(particleSet);

		double mean_x=0;
		double mean_y=0;

		for (const auto& part : particleSet)
		{
			mean_x+=part.x_cm;
			mean_y+=part.y_cm;
		}

		str::particles newParticle;

		for (size_t idx=0; idx<particleSet.size();++idx)
		{
			newParticle.push_back(str::particle(particleSet[idx].x_cm,particleSet[idx].y_cm,particleSet[idx].theta_deg));
		}
		std::cout<<"\nOdom x: "<<odomData[i].x_cm<<" meanX "<<mean_x/N_Particles<<" Mean y"<<mean_y/N_Particles;

		// grapher.setMeasuredRanges(laserData[i].r);
		// grapher.setPredictedRanges(laserData[i+4].r);
		// grapher.updateSensorGraphics();


		grapher.setParticlePoints(particleSet);
		//grapher.setLaserLines(laserData[i].r, 300, 300);

		// grapher.setParticlePoints(particleSet);
		//grapher.setLaserLines(laserData[i].r, 300, 300);
		grapher.updateGraphics();
	}



 



	return 0;
}
