#include <iostream>


#include <boost/tuple/tuple.hpp>
#include <boost/array.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/range/irange.hpp>
#include <boost/bind.hpp>


#include "gnuplot-iostream.h"
#include "str_ray_tracer.h"

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
    float map[8][11] = 
        {
            {-1,-1,-1,-1,-1, 1, 1,-1,-1,-1,-1},
            {-1,-1,-1,-1,-1, 1, 1,-1,-1, 1,-1},
            {-1, 1, 1, 1, 1, 0, 1,-1, 1, 0, 1},
            {-1, 1, 0, 0, 0, 0, 1,-1, 1, 0, 1},
            {-1,-1, 1, 0, 0, 0, 1, 1, 1, 0, 1},
            {-1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {-1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
        };
    str::RayTracer RT;
    // RT.populateRangeCache(map);
    std::vector<double> v = RT.getRangesFromPoint(map, std::pair<int,int>(4,4));
    printv(v);


    Gnuplot gp;
    std::vector<std::pair<float, float>> plotMap;
    int deg = 0;
    for(auto i=v.begin(); i!=v.end(); ++i)
    {
        double rad = (deg * PI)/180.;
        plotMap.push_back(std::pair<float,float>((*i)*cos(rad),(*i)*sin(rad)));
        deg++;
    }
    gp << "set term x11 3\n";
    // gp << "set xrange [-20:20]\nset yrange [-10:10]\n";
    gp << "plot '-' title 'prob'\n";
    gp.send1d(plotMap);
}