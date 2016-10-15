#ifndef __str_ray_tracer__
#define __str_ray_tracer__

#include <vector>
#include <cmath>
#include <unordered_map>

#define PI 3.1415926535897932384626433832795
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
    void printv(std::vector<double> r)
    {
        int th=0;
        for(auto i=r.begin(); i!=r.end(); i++)
            std::cout<<"th: "<< th++<< "range: "<<*i<<'\n';
        std::cout<<'\n';
    }

    int getQuadrant(double angle)
    {
        return angle/90;        
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
                angle_cache[angle].radians = angle*PI/180.0;
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
                    case 3:
                        stepX = 1;
                        stepY = 1;
                        break;
                    case 2:
                        stepX = -1;
                        stepY = 1;
                        break;
                    case 1:
                        stepX = -1;
                        stepY = -1;
                        break;
                    case 0:
                        stepX = 1;
                        stepY = -1;
                        break;
                }
        }

        // Populate the cache and return the ranges from the particle/point
        std::vector<double> getRangesFromPoint(float map[][C], std::pair<int,int> point)
        {
            // TODO Use from Map structure
            int map_size_x = C;
            int map_size_y = R;

            int stepX = 1;
            int stepY = 1;

            std::vector<double> ranges(MAX_DEGREES, -1);
            for(int theta=0; theta<MAX_DEGREES; theta++)
            {
                angle_data &d = angle_cache[theta];
                int X = point.second;
                int Y = point.first;
                double tmaxX = d.tmaxX;
                double tmaxY = d.tmaxY;
                double range_x = 0;
                double range_y = 0;
                setStepXY(theta, stepX, stepY);
                
                while(X>=0 && X<map_size_x && Y>=0 && Y<map_size_y)
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

                    // TODO(Tushar) handle 0.5, -1 probability
                    if(map[Y][X] == 1)
                        break;
                }
                // std::cout<<"theta: "<<theta<<", end point: "<<X<<", "<<Y<<'\n';
                
                // Select accurate range based on the octant
                switch((theta%180)/45)
                {
                    case 0:
                    case 3:
                        ranges[theta] = range_x - 0.5; 
                        // Alternate:
                        // (abs(X-point.second)*angle_cache[theta].tDeltaX) - 0.5;
                        break;
                    case 1:
                    case 2:
                        ranges[theta] = range_y - 0.5; 
                        // ALternate
                        //(abs(Y-point.first)*angle_cache[theta].tDeltaY) - 0.5;
                        break;
                }                    
                // cout<<"theta: "<<theta<<" stop point "<<Y<<", "<<X<<"range is "<<ranges[theta]<<'\n';

            }
            range_cache[point] = ranges;
            return ranges;
        }

        // Force populate the cache
        void populateRangeCache(float map[][C])
        {
            for(int i=0; i<R; i+=5)
                for(int j=0; j<C; j+=5)
                {
                    if(map[i][j] == 0 || map[i][j] == 0.5)
                        getRangesFromPoint(map, std::pair<int, int>(i,j));
                }
        }

    };
}