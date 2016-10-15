#ifndef __bee_map__
#define __bee_map__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int resolution, size_x, size_y;
	float offset_x, offset_y;
	int min_x, max_x, min_y, max_y;
	float **prob;
} map_type;

void new_hornetsoft_map(map_type *map, int size_x, int size_y)
{
	int i;
	
	map->prob = (float **)calloc(size_x, sizeof(float *));
	for(i = 0; i < size_x; i++)
		map->prob[i] = (float *)calloc(size_y, sizeof(float));
}


#endif