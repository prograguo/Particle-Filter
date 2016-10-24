#include <iostream>


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

    std::vector<str::laser> laserData;
    std::vector<str::odom> odomData;
    str::readRobotData("data/log/ascii-robotdata2.log", laserData,  odomData);

    str::Grapher grapher(costMap.size_x, costMap.size_x, 900);
    grapher.setMap(costMap.prob);

    for( auto it = laserData.begin(); it!= laserData.end(); it++)
    {
        double prob = observationModel.getProbForParticle(p, *it, costMap, grapher);
        std::cout<<"Prob for test particle is "<<prob<<'\n';
    }


    return 0;
}