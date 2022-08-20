#ifndef FMATH_H
#define FMATH_H

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include "types.h"
#include "direction.h"

#define IVEC2S2V(_v) ((vec2s) {{ (_v).x, (_v).y }})
#define VEC2S2I(_v) ((ivec2s) {{ (s32)floorf((_v).x), (s32)floorf((_v).y) }})
#define IVEC3S2V(_v) ((vec3s) {{ (_v).x, (_v).y, (_v).z }})
#define VEC3S2I(_v) ((ivec3s) {{ (s32)floorf((_v).x), (s32)floorf((_v).y), (s32)floorf((_v).z) }})

#define PI_2 GLM_PI_2
#define PI GLM_PI
#define TAU (2 * PI)
#define F32_EPSILON FLT_EPSILON
#define F64_EPSILON DBL_EPSILON

#define cosf(_x) ((f32)cos(_x))
#define sinf(_x) ((f32)sin(_x))
#define radians(_x) ((_x) * (PI / 180.0f))
#define degrees(_x) ((_x) * (180.0f / PI))
#define floori(_x) ((s32)(floor((f64)(_x))))

#define sign(_x) ({\
    __typeof__(_x) _xx = (_x);\
    ((__typeof__(_x)) ( (((__typeof__(_x)) 0) < _xx) - (_xx < ((__typeof__(_x))0)))); })

#define max(_a, _b) ({\
    __typeof__(_a) __a = (_a); \
    __typeof__(_b) __b = (_b); \
    __a > __b ? __a : __b; })

#define min(_a, _b) ({\
    __typeof__(_a) __a = (_a); \
    __typeof__(_b) __b = (_b); \
    __a < __b ? __a : __b; })

#define clamp(_x, _mn, _mx) ({\
    __typeof__(_x) __x = (_x); \
    __typeof__(_mn) __mn = (_mn); \
    __typeof__(_mx) __mx = (_mx); \
    max(__mn, min(__mx, __x)); })

typedef struct Ray {
    vec3s origin, direction;
} Ray;

bool ray_intersect_block(Ray r, ivec3s* pos_out, Direction* d_out);
s32 ivec3scmp(ivec3s a, ivec3s b);

#endif
