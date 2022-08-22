#ifndef MESH_H
#define MESH_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "../gfx/vao.h"
#include "../gfx/bo.h"

typedef struct MeshBuffer {
	void* data;
	u64 count;
} MeshBuffer;

typedef struct Chunk Chunk;

typedef struct Mesh {
	MeshBuffer data, indices;
	VAO vao;
	BO vbo, ebo;
    u64 vertex_count;
    bool dirty;
    Chunk* chunk;
} Mesh;

void mesh_emit_face(Mesh* mesh, vec3s pos, Direction dir, vec2s uv_offset, vec2s uv_unit);
void mesh_init(Mesh* self, Chunk* chunk);
void mesh_prepare(Mesh* self);
void mesh_finalize(Mesh* self);
void mesh_render(Mesh* self);
void mesh_destroy(Mesh* self);

#endif
