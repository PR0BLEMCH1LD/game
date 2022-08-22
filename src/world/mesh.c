#include "mesh.h"
#include "../gfx/shader.h"
#include "../state.h"

#define DATA_BUFFER_SIZE 16 * 256 * 16 * 120
#define INDICES_BUFFER_SIZE 16 * 256 * 16 * 36

const u8 FACE_INDICES[] = { 1, 0, 3, 1, 3, 2 };
const u8 UNIQUE_INDICES[] = { 1, 0, 5, 2 };
const u8 CUBE_INDICES[] = {
	1, 0, 3, 1, 3, 2,
	4, 5, 6, 4, 6, 7, 
	5, 1, 2, 5, 2, 6, 
	0, 4, 7, 0, 7, 3,
	2, 3, 7, 2, 7, 6, 
	5, 4, 0, 5, 0, 1  
};
const f32 CUBE_VERTICES[] = {
	0.0f, 0.0f, 0.0f,
	1.0f, 0.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	0.0f, 1.0f, 0.0f,

	0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	0.0f, 1.0f, 1.0f
};
const f32 CUBE_UVS[] = {
	1.0f, 0.0f,
	0.0f, 0.0f,
	0.0f, 1.0f,
	1.0f, 1.0f
};

struct {
	f32 data[DATA_BUFFER_SIZE];
	u32 indices[INDICES_BUFFER_SIZE];
} global_buffer;

void mesh_emit_face(Mesh* mesh, vec3s pos, Direction dir, vec2s uv_offset, vec2s uv_unit) {
	for (u8 idx = 0; idx < 4; idx++) {
		const f32 *vertex = &CUBE_VERTICES[CUBE_INDICES[(dir * 6) + UNIQUE_INDICES[idx]] * 3];
		((f32*)mesh->data.data)[mesh->data.count++] = pos.x + vertex[0];
		((f32*)mesh->data.data)[mesh->data.count++] = pos.y + vertex[1];
		((f32*)mesh->data.data)[mesh->data.count++] = pos.z + vertex[2];
		((f32*)mesh->data.data)[mesh->data.count++] = uv_offset.x + uv_unit.x * CUBE_UVS[(idx * 2) + 0];
		((f32*)mesh->data.data)[mesh->data.count++] = uv_offset.y + uv_unit.y * CUBE_UVS[(idx * 2) + 1];
	}

	for (u8 idx = 0; idx < 6; idx++) {
		((u32*)mesh->indices.data)[mesh->indices.count++] = mesh->vertex_count + FACE_INDICES[idx];
	}

	mesh->vertex_count += 4;
}

void mesh_init(Mesh* self, Chunk* chunk) {
	memset(self, 0, sizeof(Mesh));
    vao_init(&self->vao);
	bo_init(&self->vbo, GL_ARRAY_BUFFER);
	bo_init(&self->ebo, GL_ELEMENT_ARRAY_BUFFER);
    self->chunk = chunk;
} 

void mesh_prepare(Mesh* self) {
	self->vertex_count = 0;

	MeshBuffer* buffers[2] = {
		&self->data, &self->indices
	};

	for (u8 idx = 0; idx < 2; idx++) {
		MeshBuffer* buffer = buffers[idx];

		buffer->count = 0;
		buffer->data = (void* [2]) {
			global_buffer.data,
			global_buffer.indices
		}[idx];
	}
}

void mesh_finalize(Mesh* self) {
	bo_buffer(&self->vbo, self->data.data, self->data.count * sizeof(f32));
	bo_buffer(&self->ebo, self->indices.data, self->indices.count * sizeof(u32));	
}

void mesh_render(Mesh* self) {
	shader_bind(&state.shader);

    shader_uniform_mat4(&state.shader, "m", glms_translate(glms_mat4_identity(), IVEC3S2V(self->chunk->position)));
	shader_uniform_camera(&state.shader, &state.world.player.camera);
	shader_uniform_texture2D(&state.shader, "tex", &state.atlas.texture, 0);	

	const u8 data_stride = 5 * sizeof(f32);

	vao_attrib(&self->vao, &self->vbo, 0, 3, GL_FLOAT, data_stride, 0 * sizeof(f32));
	vao_attrib(&self->vao, &self->vbo, 1, 2, GL_FLOAT, data_stride, 3 * sizeof(f32));

	vao_bind(&self->vao);
	bo_bind(&self->ebo);

	glDrawElements(GL_TRIANGLES, self->indices.count, GL_UNSIGNED_INT, NULL);
}

void mesh_destroy(Mesh* self) {
	vao_destroy(&self->vao);
	bo_destroy(&self->vbo);
	bo_destroy(&self->ebo);
}
