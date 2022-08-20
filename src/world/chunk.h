#ifndef CHUNK_H
#define CHUNK_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "mesh.h"

#define CHUNK_SIZE ((ivec3s) {{ 16, 256, 16 }})
#define CHUNK_SIZE_F ((vec3s) {{ 16.0f, 256.0f, 16.0f }})
#define CHUNK_VOLUME (CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z)

#define BPOS2CI(_p) ((_p).z * CHUNK_SIZE.y * CHUNK_SIZE.x + (_p).y * CHUNK_SIZE.x + (_p).x)

#define chunk_foreach(_pname)\
    ivec3s _pname = GLMS_IVEC3_ZERO;\
    for (s32 x = 0; x < CHUNK_SIZE.x; x++)\
        for (s32 y = 0; y < CHUNK_SIZE.y; y++)\
            for (s32 z = 0;\
                z < CHUNK_SIZE.z &&\
                ((_pname.x = x) != INT32_MAX) &&\
                ((_pname.y = y) != INT32_MAX) &&\
                ((_pname.z = z) != INT32_MAX);\
                z++)

typedef struct Chunk {
    Mesh mesh;
    u32* data;
    ivec3s position;
} Chunk;

void chunk_set_data(Chunk* self, ivec3s pos, u32 data);
u32 chunk_get_data(Chunk* self, ivec3s pos);
void chunk_init(Chunk* self, ivec3s pos);
void chunk_render(Chunk* self);
void chunk_destroy(Chunk* self);

#endif
