#ifndef DIRECTION_H
#define DIRECTION_H

#include <cglm/cglm.h>
#include <cglm/struct.h>

#define DIR2IVEC3S(d) (DIRECTION_IVEC[d])
#define DIR2VEC3S(d) (DIRECTION_VEC[d])

typedef enum Direction {
	NORTH = 0,
	SOUTH,
	EAST,
	WEST,
	UP,
	DOWN
} Direction;

extern const ivec3s DIRECTION_IVEC[6];
extern const vec3s DIRECTION_VEC[6];

Direction ivec3s2dir(ivec3s v);

#endif
