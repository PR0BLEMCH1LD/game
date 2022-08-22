#ifndef ATLAS_H
#define ATLAS_H

#include "gfx.h"
#include "../util/util.h"

#include "texture.h"

typedef struct Atlas {
    Texture texture;
    ivec2s sprite_size, size;
    vec2s sprite_unit;     
} Atlas;

vec2s atlas_get_sprite_offset(Atlas* self, ivec2s pos);
void atlas_init(Atlas* self, const char* path);
void atlas_destroy(Atlas* self);

#endif
