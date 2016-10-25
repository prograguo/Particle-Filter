#include "str_ray_tracer.h"

namespace str
{

	RayTracer::RayTracer()
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

	void RayTracer::clearRangeCache()
	{
		range_cache.clear();
	}

        // Set StepXY for ray tracing based on the quadrant of the ray
	void RayTracer::setStepXY(double theta, int &stepX, int &stepY)
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
    void RayTracer::getRangesFromPoint(map_type &map, particle &p, std::vector<int> &ranges) //std::pair<int,int> point)
    {
    	std::pair<int,int> point = cmToMapCoordinates(p.x_cm, p.y_cm, map.resolution);

    	std::vector<int> ranges_tmp(MAX_DEGREES, -1);
        // Check if value exists in the cache
    	auto cached_value = range_cache.find(point);
        if(cached_value != range_cache.end()) // Exists in cache
        {
            // std::cout<<"Range cache hit\n";
        	ranges = cached_value->second;
        }
        else
        {
        	int map_size_x = map.size_x;
        	int map_size_y = map.size_y;

        	int stepX = 1;
        	int stepY = 1;
        	double range_map_coords = 0;
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
                            // range_map_coords = range_x - 0.5; 
                            // range_map_coords = (abs(X-point.first)*angle_cache[theta].tDeltaX) - 0.5;
        			range_map_coords = sqrt((X-point.first)*(X-point.first) + (Y-point.second)*(Y-point.second)) - 0.5;
        			break;
        			case 1:
        			case 2:
                            // Options:
                            // range_map_coords = range_y - 0.5;
                            // range_map_coords = (abs(Y-point.second)*angle_cache[theta].tDeltaY) - 0.5;
        			range_map_coords = sqrt((X-point.first)*(X-point.first) + (Y-point.second)*(Y-point.second))  - 0.5;
        			break;
        		}
        		int range_int = round(mapToCmCoordinates(range_map_coords, map.resolution));
        		if(range_int > MAX_RANGE)
        			range_int = MAX_RANGE;
        		ranges_tmp[theta] = range_int;
                    // std::cout<<"theta: "<<theta<<" stop point "<<Y<<", "<<X<<"range is "<<ranges[theta]<<'\n';
        	}
        	range_cache[point] = ranges_tmp;
        	ranges = ranges_tmp;
        }
    }

        // Force populate the cache
    void RayTracer::populateRangeCache(map_type &map)
    {
    	for(int i=0; i<map.size_y; i+=CACHE_RESOLUTION_X)
    	{
    		for(int j=0; j<map.size_x; j+=CACHE_RESOLUTION_Y)
    		{
    			if( !isObstacle(map.prob[i][j]) )
    			{
    				std::vector<int> ranges;
    				particle p(i, j, 0);
    				getRangesFromPoint(map, p, ranges);
    			}
    		}
    	}

    };


}//end namespace str