#include "world.h"
#include "worldgen.h"

#define CHUNKS_SIZE(_w) ((_w)->render_distance * 2 + 1)
#define NUM_CHUNKS(_w) (CHUNKS_SIZE((_w)) * CHUNKS_SIZE((_w)))

#define CPOS2COFF(_p) ((ivec3s) {{ (s32)floorf((_p).x / CHUNK_SIZE_F.x), 0, (s32)floorf((_p).z / CHUNK_SIZE_F.z) }})
#define CIDX2COFF(_w, _i) glms_ivec3_add((_w)->first_chunk, (ivec3s) {{ (_i) % CHUNKS_SIZE((_w)), 0, (_i) / CHUNKS_SIZE((_w)) }})
#define CPOS2CIDX(_w, _p) ({ ivec3s p = glms_ivec3_sub(CPOS2COFF((_p)), (_w)->first_chunk); p.z * CHUNKS_SIZE((_w)) + p.x; })
#define POS2BPOS(_p) glms_ivec3_mod(glms_ivec3_add(glms_ivec3_mod(_p, CHUNK_SIZE), CHUNK_SIZE), CHUNK_SIZE)

#define world_foreach(_w, _cname)\
    Chunk* _cname;\
    for (u64 idx = 0; idx < NUM_CHUNKS(_w) &&\
        (_cname = (_w)->chunks[idx]) != (void *)UINT64_MAX;\
        idx++)

static s32 _WorldModifiedDatacmp(gconstpointer a, gconstpointer b) {
    const WorldModifiedData* _a = a, * _b = b;

    return ivec3scmp(_a->position, _b->position);
}

static void _load_chunk(World* self, u64 idx) {
    Chunk* chunk = malloc(sizeof(Chunk));

    chunk_init(chunk, glms_ivec3_mul(CIDX2COFF(self, idx), CHUNK_SIZE));
    worldgen_generate(chunk);

    self->chunks[idx] = chunk;

    for (u64 i = 0; i < self->modified_data->len; i++) {
        WorldModifiedData mdata = g_array_index(self->modified_data, WorldModifiedData, i);
        Chunk* c = world_get_chunk(self, mdata.position);

        if (c != NULL && !ivec3scmp(c->position, chunk->position)) {
            world_set_data(self, mdata.position, mdata.data);
        } 
    }
}

static void _load_empty_chunks(World* self) {
    for (u64 idx = 0; idx < NUM_CHUNKS(self); idx++) {
        if (self->chunks[idx] == NULL) {
            _load_chunk(self, idx);
        }
    }
}

bool world_chunk_in_bounds(World *self, ivec3s pos) {
    ivec3s p = glms_ivec3_sub(CPOS2COFF(pos), self->first_chunk);

    return p.x >= 0 && p.z >= 0 && p.x < (s32)CHUNKS_SIZE(self) && p.z < (s32)CHUNKS_SIZE(self);
}

Chunk* world_get_chunk(World* self, ivec3s pos) {
    if (!world_chunk_in_bounds(self, pos)) {
        return NULL;
    }

    return self->chunks[CPOS2CIDX(self, pos)];
}

void world_set_data(World* self, ivec3s pos, BlockId data) {
    if (world_get_chunk(self, pos) != NULL) {
        chunk_set_data(world_get_chunk(self, pos), POS2BPOS(pos), data);
    }
}

BlockId world_get_data(World* self, ivec3s pos) {
    if (pos.y >= 0 && pos.y < CHUNK_SIZE.y && world_get_chunk(self, pos) != NULL) {
        return chunk_get_data(world_get_chunk(self, pos), POS2BPOS(pos));
    }

    return 0;
}

void world_set_center(World* self, ivec3s pos) {
    ivec3s new_first_chunk = glms_ivec3_sub(
        CPOS2COFF(pos),
        (ivec3s) {{ CHUNKS_SIZE(self) / 2, 0, CHUNKS_SIZE(self) / 2 }}
    );

    if (!ivec3scmp(new_first_chunk, self->first_chunk)) {
        return;
    }

    self->first_chunk = new_first_chunk;

    Chunk* old_chunks[NUM_CHUNKS(self)];

    memcpy(old_chunks, self->chunks, NUM_CHUNKS(self) * sizeof(Chunk*));
    memset(self->chunks, 0, NUM_CHUNKS(self) * sizeof(Chunk*));

    for (u64 idx = 0; idx < NUM_CHUNKS(self); idx++) {
        Chunk* chunk = old_chunks[idx];

        if (chunk == NULL) {
            continue;
        } 
        else if (world_chunk_in_bounds(self, chunk->position)) {
            u32 i = CPOS2CIDX(self, chunk->position);

            if (i >= NUM_CHUNKS(self) - CHUNKS_SIZE(self) || (i + 1) % CHUNKS_SIZE(self) == 0) {
                chunk->mesh.dirty = true;
            }

            self->chunks[i] = chunk;
        }
        else {
            chunk_destroy(chunk);
            free(chunk);
        }
    }

    _load_empty_chunks(self);
}

void world_append_modified_data(World* self, WorldModifiedData mdata) {
    u32 idx;

    if (g_array_binary_search(self->modified_data, &mdata, _WorldModifiedDatacmp, &idx)) {
        g_array_index(self->modified_data, WorldModifiedData, idx) = mdata;
    }
    else {
        g_array_append_val(self->modified_data, mdata);
    }

    g_array_sort(self->modified_data, _WorldModifiedDatacmp);
}

void world_init(World* self) {
	memset(self, 0, sizeof(World));
    player_init(&self->player);
    self->render_distance = 4;
    self->chunks = calloc(NUM_CHUNKS(self), sizeof(Chunk*));
    self->modified_data = g_array_new(false, false, sizeof(WorldModifiedData));

    world_set_center(self, GLMS_IVEC3_ZERO);
}

void world_tick(World* self) {
	player_tick(&self->player);
}

void world_update(World* self) {
	player_update(&self->player);
}

void world_render(World* self) {
    world_foreach(self, chunk) {
        if (chunk != NULL) {
            chunk_render(chunk);
        }
    }
}

void world_destroy(World* self) {
    world_foreach(self, chunk) {
        if (chunk != NULL) {
            chunk_destroy(chunk);
            free(chunk);
        }
    }

    free(self->chunks);
    g_array_free(self->modified_data, true);
}
