#include <iostream>
#include <unistd.h> // usleep

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>
#include <libconfig.h++>


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
    libconfig::Config cfg;
    cfg.readFile("config/params.cfg");

    // Load Map
    map_type costMap;
    std::string dataLoc = cfg.lookup("mapFile");
    int val = read_beesoft_map(&dataLoc[0], &costMap);

    // Load Sensor Logs
    std::vector<str::laser> laserData;
    std::vector<str::odom> odomData;
    std::string logfile = cfg.lookup("logFile");
    str::readRobotData(logfile, laserData,  odomData);
    
    str::sensor_model_params sensorParams;
      sensorParams.uniformParam = cfg.lookup("sensorModel.uniformParam");
      sensorParams.decayScale =   cfg.lookup("sensorModel.decayScale");
      sensorParams.decayRate =    cfg.lookup("sensorModel.decayRate");
      sensorParams.maxParam =     cfg.lookup("sensorModel.maxParam");
      sensorParams.rangeSTD =     cfg.lookup("sensorModel.rangeSTD");
      sensorParams.gaussianGain = cfg.lookup("sensorModel.gaussianGain");
    str::observation_model observationModel(sensorParams);

    std::vector<double> sModel;
    observationModel.getSensorModel(sModel);

    
    Gnuplot gp;

    gp << "plot '-' title 'prob'\n";
    gp.send1d(sModel);


    //char c;
    //std::cout << "Any key to continue\n";
    //std::cin >> c;

    //str::Grapher grapher_temp{costMap.size_x, costMap.size_x, 300}; 
    //str::Grapher& grapher = grapher_temp;
    //grapher.setMap(costMap.prob);
    //grapher.updateGraphics();

    observationModel.forcePopulateRangeCache(costMap);

    str::particle p(2300, 4150, 0);

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
        str::particle newParticle(2300, 4150, r_pt);
        particleSet.push_back(newParticle);
    }


    str::Grapher grapher(costMap.size_x, costMap.size_x, 300);
    grapher.setMap(costMap.prob);
    

    for( auto it = laserData.begin(); it!= laserData.end()-1; it++)
    {

        for( auto pit = particleSet.begin(); pit != particleSet.end(); pit++)
        {   
            grapher.setLaserLines(it->r, *pit);
            for(int i = 0 ; i < 10; i++){
                pit->theta_rad += 0.03;
                double prob = observationModel.getProbForParticle(*pit, *it, costMap, grapher, 1);
                std::cout<<"Prob for test particle is "<<prob<<'\n';
                grapher.setParticlePoints(particleSet);
                grapher.updateGraphics();
                usleep(10);
            }
        }
    }


    return 0;
}