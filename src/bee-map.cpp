#include "bee-map.h"


void new_hornetsoft_map(map_type *map, int size_x, int size_y)
{
	int i;
	
	map->prob = (float **)calloc(size_x, sizeof(float *));
	for(i = 0; i < size_x; i++)
		map->prob[i] = (float *)calloc(size_y, sizeof(float));
}