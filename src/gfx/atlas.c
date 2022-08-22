#include "atlas.h"

vec2s atlas_get_sprite_offset(Atlas* self, ivec2s pos) {
    return glms_vec2_mul((vec2s) {{ pos.x, self->size.y - 1 - pos.y }}, self->sprite_unit);
}

void atlas_init(Atlas* self, const char* path) {
    memset(self, 0, sizeof(Atlas));
    texture_init(&self->texture, path);
    self->sprite_size = (ivec2s) {{ 16, 16 }};
    self->size = glms_ivec2_div(self->texture.size, self->sprite_size);
    self->sprite_unit = glms_vec2_div(IVEC2S2V(self->sprite_size), IVEC2S2V(self->texture.size));
}

void atlas_destroy(Atlas* self) {
    texture_destroy(&self->texture);
}
