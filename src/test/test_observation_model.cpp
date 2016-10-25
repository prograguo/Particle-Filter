#include <iostream>
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
#include "str_observation_model.h"

#include "bee-map.h"
#include "bee-map-impl.h"


int main()
{
    map_type costMap;

    // TEST MAP:
    // char datLoc[] = "data/map/test_ray_tracer.dat";
    // int val = read_beesoft_map(datLoc, &costMap);

    // WEAN:
    char datLoc[] = "data/map/wean.dat";
    int val = read_beesoft_map(datLoc, &costMap);
    
    str::sensor_model_params sensorParams;
    sensorParams.uniformParam = 0.0001;
    sensorParams.decayScale = 1000;
    sensorParams.decayRate = 0.0001;
    sensorParams.maxParam = 1000;
    sensorParams.rangeSTD = 250;

    str::observation_model observationModel(sensorParams);

    observationModel.forcePopulateRangeCache(costMap);

    str::particle p(230, 415, 0);

    std::vector<std::pair<int, int>> freeSpace;
    unsigned int width = costMap.size_x;
    unsigned int height = costMap.size_y;
    for (unsigned int i = 0; i < width; ++i)
    {
      for (unsigned int j = 0; j < height; ++j)
      {
        if(costMap.prob[i][j] == 0){
            freeSpace.push_back(std::pair<int, int>(i,j));
        }
      }
    }
    int N_Particles = 1;
    str::particles particleSet;
    for (unsigned int i = 0; i < N_Particles; i++)
    {
        std::pair<int, int> pt;
        int r_pt = (std::rand() * freeSpace.size()) / RAND_MAX ;
        pt = freeSpace[r_pt];
        str::particle newParticle(pt.first, pt.second, r_pt);
        particleSet.push_back(newParticle);
    }

    std::vector<str::laser> laserData;
    std::vector<str::odom> odomData;
    str::readRobotData("data/log/ascii-robotdata2.log", laserData,  odomData);

    str::Grapher grapher(costMap.size_x, costMap.size_x, 300);
    grapher.setMap(costMap.prob);
    

    for( auto it = laserData.begin(); it!= laserData.end(); it++)
    {
        for( auto pit = particleSet.begin(); pit != particleSet.end(); pit++)
        {   
            pit->theta_rad += 0.03;
            double prob = observationModel.getProbForParticle(*pit, *it, costMap, grapher);
            std::cout<<"Prob for test particle is "<<prob<<'\n';
            grapher.setParticlePoints(particleSet);
            grapher.updateGraphics();
            usleep(1000);
        }
    }


    return 0;
}