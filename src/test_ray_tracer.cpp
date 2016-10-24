#include <iostream>


#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>


#include "gnuplot-iostream.h"
#include "str_ray_tracer.h"
#include "types.h"
#include "str_io.h"
#include "str_sensor_model.h"
#include "helper_functions.h"

#include "bee-map.h"
#include "bee-map-impl.h"

void printv(std::vector<double> r)
{
    int th=0;
    for(auto i=r.begin(); i!=r.end(); i++)
        std::cout<<"th: "<< th++<< ", range: "<<*i<<'\n';
    std::cout<<'\n';
}


int main()
{
    // int map[R][C] = {0};
    // float map[8][11] = 
    //     {
    //         {-1,-1,-1,-1,-1, 1, 1,-1,-1,-1,-1},
    //         {-1,-1,-1,-1,-1, 1, 1,-1,-1, 1,-1},
    //         {-1, 1, 1, 1, 1, 0, 1,-1, 1, 0, 1},
    //         {-1, 1, 0, 0, 0, 0, 1,-1, 1, 0, 1},
    //         {-1,-1, 1, 0, 0, 0, 1, 1, 1, 0, 1},
    //         {-1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //         {-1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    //         { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    //     };
    str::RayTracer RT;
    // RT.populateRangeCache(map);

    


    map_type costMap;

    // TEST MAP:
    // char datLoc[] = "data/map/test_ray_tracer.dat";
    // int val = read_beesoft_map(datLoc, &costMap);
    // str::particle p(4, 5, 0);
    // std::vector<double> v;
    // RT.getRangesFromPoint(costMap, p, v);

    // WEAN:
    char datLoc[] = "data/map/wean.dat";
    int val = read_beesoft_map(datLoc, &costMap);
    str::particle p(230, 415, 0);
    std::vector<int> v;
    RT.getRangesFromPoint(costMap, p, v);
    // printv(v);


    std::cout<<"Map size: "<<costMap.size_x<<", "<<costMap.size_y<<'\n';
    Gnuplot gp;
    
    // Plot the test map
    std::vector<std::pair<float, float>> plotMap1;
    for(int i = 0; i < costMap.size_x; i++)
        for(int j = 0; j < costMap.size_y; j++)
        {
            if(costMap.prob[i][j] == 0) {
                plotMap1.push_back(std::pair<float,float>(float(i),float(j)));
            }
            // else
                // std::cout<<"Valid point: "<<i<<", "<<j<<costMap.prob[i][j]<<"\n";
        }
    gp << "set term x11 1\n";
    // gp << "set xrange [0:"<<costMap.size_x<<"]\nset yrange [0:"<<costMap.size_y<<"]\n";
    gp << "plot '-' title 'obstacle'\n";
    gp.send1d(plotMap1);


    // Plot the lidar output
    std::vector<std::pair<float, float>> plotMap2;
    int deg = 0;
    for(auto i=v.begin(); i!=v.end(); ++i)
    {
        double rad = (deg * M_PI)/180.;
        plotMap2.push_back(std::pair<float,float>((*i)*cos(rad),(*i)*sin(rad)));
        deg++;
    }
    gp << "set term x11 2\n";
    // gp << "set xrange [-20:20]\nset yrange [-10:10]\n";
    gp << "plot '-' title 'prob'\n";
    gp.send1d(plotMap2);
}