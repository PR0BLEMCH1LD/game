#ifndef WORLD_H
#define WORLD_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "../entity/player.h"
#include "chunk.h"
#include "../block/block.h"

typedef struct WorldModifiedData {
    ivec3s position;
    BlockId data;
} WorldModifiedData;

typedef struct World {
	Player player;
	Chunk** chunks;
    u32 render_distance;
    ivec3s first_chunk;
    GArray* modified_data;
} World;

bool world_chunk_in_bounds(World* self, ivec3s pos);
Chunk* world_get_chunk(World* self, ivec3s pos);
void world_set_data(World* self, ivec3s pos, BlockId data);
BlockId world_get_data(World* self, ivec3s pos);
void world_set_center(World* self, ivec3s pos);
void world_append_modified_data(World* self, WorldModifiedData mdata);
void world_init(World* self);
void world_update(World* self);
void world_tick(World* self);
void world_render(World* self);
void world_destroy(World* self);

#endif
