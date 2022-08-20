#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void texture_bind(Texture* self) {
	glBindTexture(GL_TEXTURE_2D, self->handle);
}

void texture_init(Texture* self, const char* path) {
    memset(self, 0, sizeof(Texture));
    glGenTextures(1, &self->handle);

	stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
	unsigned char* image = stbi_load(path, &width, &height, &channels, 0);

    texture_bind(self);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image);
}

void texture_destroy(Texture* self) {
	glDeleteTextures(1, &self->handle);
}
