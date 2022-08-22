#ifndef TEXTURE_H
#define TEXTURE_H

#include "gfx.h"
#include "../util/util.h"

typedef struct Texture {
	GLuint handle;
    ivec2s size;
} Texture;

void texture_bind(Texture* self);
void texture_init(Texture* self, const char* path);
void texture_destroy(Texture* self);

#endif
