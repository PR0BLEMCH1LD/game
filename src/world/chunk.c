#include "chunk.h"
#include "world.h"
#include "../state.h"

static bool _block_in_bounds(ivec3s pos) {
    return pos.x >= 0 && pos.y >= 0 && pos.z >= 0 &&
        pos.x < CHUNK_SIZE.x && pos.y < CHUNK_SIZE.y && pos.z < CHUNK_SIZE.z;
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
		u32 data = self->data[BPOS2CI(pos)];

		if (data != 0) {
			for (Direction dir = 0; dir < 6; dir++) {
                ivec3s dvec = DIR2IVEC3S(dir);
				ivec3s neighbor = glms_ivec3_add(pos, dvec), wneighbor = glms_ivec3_add(wpos, dvec);

				if (_block_in_bounds(neighbor) ? 
                        self->data[BPOS2CI(neighbor)] == 0 : 
                        world_chunk_in_bounds(&state.world, wneighbor) 
                            && world_get_data(&state.world, wneighbor) == 0) {
					mesh_emit_face(&self->mesh, fpos, dir);
				}
			}
		}
	}

	mesh_finalize(&self->mesh);
}

void chunk_set_data(Chunk* self, ivec3s pos, u32 data) {
    assert(_block_in_bounds(pos));

    self->data[BPOS2CI(pos)] = data;

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

u32 chunk_get_data(Chunk* self, ivec3s pos) {
    assert(_block_in_bounds(pos));

    return self->data[BPOS2CI(pos)];
}

void chunk_init(Chunk* self, ivec3s pos) {
	memset(self, 0, sizeof(Chunk));
    mesh_init(&self->mesh, self);
	self->data = calloc(1, CHUNK_VOLUME * sizeof(u32));
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
