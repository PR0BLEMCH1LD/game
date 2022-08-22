#include "chunk.h"
#include "world.h"
#include "../state.h"
#include "../gfx/atlas.h"

#define BPOS2CIDX(_p) ((_p).z * CHUNK_SIZE.y * CHUNK_SIZE.x + (_p).y * CHUNK_SIZE.x + (_p).x)

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

static bool _block_in_bounds(ivec3s pos) {
    return pos.x >= 0 && pos.y >= 0 && pos.z >= 0 && pos.x < CHUNK_SIZE.x && pos.y < CHUNK_SIZE.y && pos.z < CHUNK_SIZE.z;
}

static bool _block_on_bounds(ivec3s pos) {
    return pos.x == 0 || pos.z == 0 || pos.x == (CHUNK_SIZE.x - 1) || pos.z == (CHUNK_SIZE.z - 1);
}

static Chunk** _get_bordering_chunks(Chunk* self, ivec3s pos) {
    Chunk** neighbors = calloc(2, sizeof(Chunk*));
    u8 idx = 0;

    if (pos.x == 0) {
        neighbors[idx++] = world_get_chunk(&state.world, glms_ivec3_add(self->position, (ivec3s) {{ -16, 0, 0 }}));
    }

    if (pos.z == 0) {
        neighbors[idx++] = world_get_chunk(&state.world, glms_ivec3_add(self->position, (ivec3s) {{ 0, 0, -16 }}));
    }

    if (pos.x == CHUNK_SIZE.x - 1) {
        neighbors[idx++] = world_get_chunk(&state.world, glms_ivec3_add(self->position, (ivec3s) {{ 16, 0, 0 }}));
    } 

    if (pos.z == CHUNK_SIZE.z - 1) {
        neighbors[idx++] = world_get_chunk(&state.world, glms_ivec3_add(self->position, (ivec3s) {{ 0, 0, 16 }}));
    }

    return neighbors;
}

static void _process(Chunk* self) {
	mesh_prepare(&self->mesh);

	chunk_foreach(pos) {
		vec3s fpos = IVEC3S2V(pos);
        ivec3s wpos = glms_ivec3_add(pos, self->position);
		BlockId data = self->data[BPOS2CIDX(pos)];

		if (data != AIR) {
			for (Direction dir = NORTH; dir <= DOWN; dir++) {
                ivec3s dvec = DIR2IVEC3S(dir);
				ivec3s neighbor = glms_ivec3_add(pos, dvec), wneighbor = glms_ivec3_add(wpos, dvec);

				if (_block_in_bounds(neighbor) ? self->data[BPOS2CIDX(neighbor)] == AIR : 
                        world_chunk_in_bounds(&state.world, wneighbor) && world_get_data(&state.world, wneighbor) == AIR) {
					mesh_emit_face(&self->mesh, fpos, dir, atlas_get_sprite_offset(&state.atlas, BLOCKS[data].texture_location), state.atlas.sprite_unit);
				}
			}
		}
	}

	mesh_finalize(&self->mesh);
}

void chunk_set_data(Chunk* self, ivec3s pos, BlockId data) {
    assert(_block_in_bounds(pos));

    self->data[BPOS2CIDX(pos)] = data;

    if (_block_on_bounds(pos)) {
        Chunk** neighbors = _get_bordering_chunks(self, pos);
        
        for (u8 idx = 0; idx < 2; idx++) {
            if (neighbors[idx] != NULL) {
                neighbors[idx]->mesh.dirty = true;
            }
        }

        free(neighbors);
    }

    self->mesh.dirty = true;
}

BlockId chunk_get_data(Chunk* self, ivec3s pos) {
    assert(_block_in_bounds(pos));

    return self->data[BPOS2CIDX(pos)];
}

void chunk_init(Chunk* self, ivec3s pos) {
	memset(self, 0, sizeof(Chunk));
    mesh_init(&self->mesh, self);
	self->data = calloc(CHUNK_VOLUME, sizeof(BlockId));
    self->position = pos;
}

void chunk_render(Chunk* self) {
    if (self->mesh.dirty) {
        self->mesh.dirty = false;

        _process(self);
    }

	mesh_render(&self->mesh);
}

void chunk_destroy(Chunk* self) {
    mesh_destroy(&self->mesh);
    free(self->data);
}
