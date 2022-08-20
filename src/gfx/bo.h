#ifndef VBO_H
#define VBO_H

#include "gfx.h"
#include "../util/util.h"

typedef struct BO {
	GLuint handle;
	GLint type;
} BO;

void bo_buffer(BO* self, void* data, GLsizeiptr size);
void bo_bind(BO* self);
void bo_init(BO* self, GLint type);
void bo_destroy(BO* self);

#endif
