#ifndef __str_ray_tracer__
#define __str_ray_tracer__
#endif

#include <vector>
#include <cmath>
#include <unordered_map>
#include "bee-map.h"

#define MAX_DEGREES 360

// TODO(Tushar): Remove afer using Map structure
#define R 8
#define C 11

namespace str {
    
    // Simple hash for pair of integers
    struct SimpleHash {
        size_t operator()(const std::pair<int, int>& p) const {
            return p.first ^ p.second;
        }
    };
    
    typedef std::unordered_map<std::pair<int, int>, std::vector<double>, SimpleHash> range_map;
    

    //
    // Helper Functions
    //
    int getQuadrant(double angle)
    {
        return angle/90;        
    }

    bool isObstacle(double value)
    {
        return value > 0.5;
    }
    //
    // Helper functions end
    //

    //
    // Trigonometric data required for each angle in degrees
    //
    struct angle_data {
        double radians;
        double tmaxX;
        double tmaxY;
        double tDeltaX;
        double tDeltaY;
    };

    //
    // Main class for RayTracer
    // Traces rays for integer degree angles and caches the results
    //
    class RayTracer {
        
        // Cache for ranges from a point in the map.
        range_map range_cache;

        // Data required per degree angle for ray tracing
        angle_data angle_cache[MAX_DEGREES];

    public:

        RayTracer()
        {
            // Initialize angle cache
            // with radians and maxXY for ray tracing
            for(int angle=0; angle<MAX_DEGREES; angle++)
            {
                // TODO(Tushar) Optimize?
                angle_cache[angle].radians = angle * M_PI/180.0;
                double tan_radians = tan(angle_cache[angle].radians);
                angle_cache[angle].tmaxX = fabs(0.5*tan_radians);
                angle_cache[angle].tmaxY = fabs(0.5/tan_radians);
                angle_cache[angle].tDeltaX = fabs(1./cos(angle_cache[angle].radians));
                angle_cache[angle].tDeltaY = fabs(1./sin(angle_cache[angle].radians));
            }
        }

        // Set StepXY for ray tracing based on the quadrant of the ray
        void setStepXY(double theta, int &stepX, int &stepY)
        {
            int quadrant = getQuadrant(theta);
            // Note: To keep angles increasing anti-clockwise, 
            // X will be decreased in quadrants 1,2 and Y in 0,1
            switch(quadrant)
                {
                    case 0:
                        stepX = 1;
                        stepY = 1;
                        break;
                    case 1:
                        stepX = -1;
                        stepY = 1;
                        break;
                    case 2:
                        stepX = -1;
                        stepY = -1;
                        break;
                    case 3:
                        stepX = 1;
                        stepY = -1;
                        break;
                }
        }

        // Populate the cache and return the ranges from the particle/point
        std::vector<double> getRangesFromPoint(map_type &map, std::pair<int,int> point)
        {
            // TODO Use from Map structure
            int map_size_x = map.size_x;
            int map_size_y = map.size_y;

            int stepX = 1;
            int stepY = 1;

            std::vector<double> ranges(MAX_DEGREES, -1);
            for(int theta=0; theta<MAX_DEGREES; theta++)
            {
                angle_data &d = angle_cache[theta];
                int X = point.first;
                int Y = point.second;
                double tmaxX = d.tmaxX;
                double tmaxY = d.tmaxY;
                double range_x = 0;
                double range_y = 0;
                setStepXY(theta, stepX, stepY);
                
                while(true)
                {
                    // std::cout<<"theta: "<<theta<<", tmaxX "<<tmaxX<<", tmaxy "<<tmaxY<<", deltax "<<d.tDeltaX<<", deltay "<<d.tDeltaY<<", range: "<<range_x<<'\n';
                    if(tmaxX < tmaxY)
                    {
                        tmaxX += d.tDeltaX;
                        range_x += d.tDeltaX;
                        X += stepX;
                    }
                    else
                    {
                        tmaxY += d.tDeltaY;
                        range_y += d.tDeltaY;
                        Y += stepY;
                    }
                    if(!(X>=0 && X<map_size_x && Y>=0 && Y<map_size_y))
                        break;
                    // TODO(Tushar) handle 0.5, -1 probability
                    if(isObstacle(map.prob[X][Y]))
                        break;
                }
                // std::cout<<"theta: "<<theta<<", end point: "<<X<<", "<<Y<<'\n';
                
                // Select accurate range based on the octant. Option 3 is independent of Octant
                switch((theta%180)/45)
                {
                    case 0:
                    case 3:
                        // Options:
                        // ranges[theta] = range_x - 0.5; 
                        // ranges[theta] = (abs(X-point.first)*angle_cache[theta].tDeltaX) - 0.5;
                        ranges[theta] = sqrt((X-point.first)*(X-point.first) + (Y-point.second)*(Y-point.second)) - 0.5;
                        break;
                    case 1:
                    case 2:
                        // Options:
                        // ranges[theta] = range_y - 0.5;
                        // ranges[theta] = (abs(Y-point.second)*angle_cache[theta].tDeltaY) - 0.5;
                        ranges[theta] = sqrt((X-point.first)*(X-point.first) + (Y-point.second)*(Y-point.second))  - 0.5;
                        break;
                }
                // cout<<"theta: "<<theta<<" stop point "<<Y<<", "<<X<<"range is "<<ranges[theta]<<'\n';

            }
            range_cache[point] = ranges;
            return ranges;
        }

        // Force populate the cache
        void populateRangeCache(map_type &map)
        {
            for(int i=0; i<map.size_y; i+=5)
                for(int j=0; j<map.size_x; j+=5)
                {
                    if( !isObstacle(map.prob[i][j]) )
                        getRangesFromPoint(map, std::pair<int, int>(i,j));
                }
        }

    };
} // end ns str