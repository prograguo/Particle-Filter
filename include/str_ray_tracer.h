#ifndef __str_ray_tracer__
#define __str_ray_tracer__

#include <vector>
#include <cmath>
#include <unordered_map>

#include "bee-map.h"
#include "particle.h"
#include "types.h"
#include "helper_functions.h"

#define MAX_DEGREES 360

#define CACHE_RESOLUTION_X 1
#define CACHE_RESOLUTION_Y 1

namespace str {
    
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

        RayTracer();

        void clearRangeCache();

        // Set StepXY for ray tracing based on the quadrant of the ray
        void setStepXY(double theta, int &stepX, int &stepY);

        // Populate the cache and return the ranges from the particle/point
        void getRangesFromPoint(map_type &map, particle &p, std::vector<int> &ranges);

        // Force populate the cache
        void populateRangeCache(map_type &map);

    };
} // end ns str
#endif