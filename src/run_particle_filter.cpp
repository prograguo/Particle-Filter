#include <iostream>
#include <unistd.h> // usleep

#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>
#include <libconfig.h++>

#include "types.h"
#include "str_io.h"
#include "bee-map.h"
#include "bee-map-impl.h"
#include "helper_functions.h"
#include "particle_filter.h"


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
    str::Grapher grapher(costMap.size_x, costMap.size_x, 300);
    grapher.setMap(costMap.prob);
    grapher.updateGraphics();

    // initialize particle filter
    str::particle_filter PF(cfg, odomData[0], costMap);
    PF.generate_random_particles();

    // visualize random particles
    str::particles particleSet;
    PF.get_particle_set(particleSet);
    grapher.setParticlePoints(particleSet);
    grapher.updateGraphics();

    for( auto it = odomData.begin(); it!= odomData.end(); it++)
    {


    }
    
    char c;
    std::cin >> c;
    return 0;
}