#include <iostream>
#include <unistd.h> // usleep

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>
#include <libconfig.h++>
#include <memory>

#include "types.h"
#include "str_io.h"
#include "bee-map.h"
#include "bee-map-impl.h"
#include "helper_functions.h"
#include "particle_filter.h"
#include "grapher.h"


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

    // Start graphics and plot the map
    // str::Grapher temp(costMap.size_x, costMap.size_x, 300); 
    // std::shared_ptr<str::Grapher> grapher= std::make_shared<str::Grapher>(temp);

    str::Grapher grapher_temp{costMap.size_x, costMap.size_x, 300};
    
    str::Grapher& grapher = grapher_temp;
    grapher.setMap(costMap.prob);
    grapher.updateGraphics();

    // auto grapher_ptr = std::make_shared<str::Grapher>(grapher);

    // std::shared_ptr<str::Grapher> grapher_ptr{grapher};
    // initialize particle filter
    str::particle_filter PF(cfg, odomData[0], costMap,grapher);
    PF.generate_random_particles();

    // visualize random particles
    str::particles particleSet;
    PF.get_particle_set(particleSet);
    grapher.setParticlePoints(particleSet);
    grapher.updateGraphics();

    std::cout<<odomData.size()<<" "<<laserData.size()<<std::endl;
    // if (odomData.size()!=laserData.size())
    // {
    // //     std::cerr<<"Laser Readings and Odom Data out of sync";
    // // }

    // // else
    // // {
     size_t l_idx = 0;
    for (size_t o_idx=0; o_idx < odomData.size(); ++o_idx)
    {

        PF.filter_update_odom(odomData[o_idx]);

        if(odomData[o_idx].ts >laserData[l_idx].ts && l_idx<laserData.size())
            {
                PF.filter_update_laser(laserData[l_idx]);
                grapher.setLaserLines(laserData[l_idx].r, particleSet[0]);
                ++l_idx;
            }
    PF.get_particle_set(particleSet);

            grapher.setParticlePoints(particleSet);
        grapher.updateGraphics();
    }
    // }
    // char c;
    // std::cin >> c;
    return 0;
}