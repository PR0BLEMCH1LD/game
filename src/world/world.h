#ifndef WORLD_H
#define WORLD_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "../entity/player.h"
#include "chunk.h"

#define NUM_CHUNKS(_w) ((_w)->chunks_size * (_w)->chunks_size)

#define CPOS2COFF(_p) ((ivec3s) {{ (s32)floorf((_p).x / CHUNK_SIZE_F.x), 0, (s32)floorf((_p).z / CHUNK_SIZE_F.z) }})
#define CIDX2COFF(_w, _i) glms_ivec3_add((_w)->first_chunk, (ivec3s) {{ (_i) % (_w)->chunks_size, 0, (_i) / (_w)->chunks_size }})
#define CPOS2CIDX(_w, _p) ({ ivec3s p = glms_ivec3_sub(CPOS2COFF((_p)), (_w)->first_chunk); p.z * (_w)->chunks_size + p.x; })
#define POS2BPOS(_p) glms_ivec3_mod(glms_ivec3_add(glms_ivec3_mod(_p, CHUNK_SIZE), CHUNK_SIZE), CHUNK_SIZE)

#define world_foreach(_w, _cname)\
    Chunk* _cname;\
    for (u64 idx = 0; idx < NUM_CHUNKS(_w) &&\
        (_cname = (_w)->chunks[idx]) != (void *) UINT64_MAX;\
        idx++)

typedef struct WorldModifiedData {
    ivec3s position;
    u32 data;
} WorldModifiedData;

typedef struct World {
	Player player;
	Chunk** chunks;
    u32 render_distance, chunks_size;
    ivec3s first_chunk;
    GArray* modified_data;
} World;

bool world_chunk_in_bounds(World* self, ivec3s pos);
Chunk* world_get_chunk(World* self, ivec3s pos);
void world_set_data(World* self, ivec3s pos, u32 data);
u32 world_get_data(World* self, ivec3s pos);
void world_set_center(World* self, ivec3s pos);
void world_append_modified_data(World* self, WorldModifiedData mdata);
void world_init(World* self);
void world_update(World* self);
void world_tick(World* self);
void world_render(World* self);
void world_destroy(World* self);

#endif
