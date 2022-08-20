#ifndef SHADER_H
#define SHADER_H

#include "gfx.h"
#include "../util/util.h"

#include "texture.h"
#include "../entity/camera.h"

typedef struct Shader {
	GLuint handle, vhandle, fhandle;
} Shader;

void shader_bind(Shader* self);
void shader_uniform_texture2D(Shader* self, const char* name, Texture* texture, GLuint offset);
void shader_uniform_mat4(Shader* self, const char* name, mat4s mat);
void shader_uniform_camera(Shader* self, Camera* camera);
void shader_init(Shader* self, const char* vs_path, const char* fs_path);
void shader_destroy(Shader* self);

#endif
