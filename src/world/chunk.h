#ifndef CHUNK_H
#define CHUNK_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "mesh.h"
#include "../block/block.h"

#define CHUNK_SIZE ((ivec3s) {{ 16, 256, 16 }})
#define CHUNK_SIZE_F ((vec3s) {{ 16.0f, 256.0f, 16.0f }})
#define CHUNK_VOLUME (CHUNK_SIZE.x * CHUNK_SIZE.y * CHUNK_SIZE.z)

typedef struct Chunk {
    Mesh mesh;
    BlockId* data;
    ivec3s position;
} Chunk;

void chunk_set_data(Chunk* self, ivec3s pos, BlockId data);
BlockId chunk_get_data(Chunk* self, ivec3s pos);
void chunk_init(Chunk* self, ivec3s pos);
void chunk_render(Chunk* self);
void chunk_destroy(Chunk* self);

#endif
