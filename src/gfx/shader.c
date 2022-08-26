#include "shader.h"

static GLuint compile(const char* path, GLenum type) {
    FILE* f;
    char* text;
    long len;

    f = fopen(path, "rb");
    fseek(f, 0, SEEK_END);
    len = ftell(f);
    assert(len > 0);
    fseek(f, 0, SEEK_SET);
    text = calloc(1, len + 1);
    text[len] = '\0';
    assert(text != NULL);
    fread(text, 1, len, f);
    assert(strlen(text) > 0);
    fclose(f);

    GLuint handle = glCreateShader(type);

    glShaderSource(handle, 1, (const GLchar* const*)&text, (const GLint*)&len);
    glCompileShader(handle);

    free(text);

    return handle;
}

void shader_bind(Shader* self) {
	glUseProgram(self->handle);
}

void shader_uniform_texture2D(Shader* self, const char* name, Texture* texture, GLuint offset) {
    glActiveTexture(GL_TEXTURE0 + offset);

    texture_bind(texture);

    glUniform1i(glGetUniformLocation(self->handle, (const GLchar*)name), offset);
}

void shader_uniform_mat4(Shader* self, const char* name, mat4s mat) {
    glUniformMatrix4fv(glGetUniformLocation(self->handle, name), 1, GL_FALSE, (const GLfloat*)&mat.raw);
}

void shader_uniform_camera(Shader* self, Camera* camera) {
    shader_uniform_mat4(self, "v", camera->view);
    shader_uniform_mat4(self, "p", camera->projection);
}

void shader_init(Shader* self, const char* vpath, const char* fpath) {
    memset(self, 0, sizeof(Shader));
	self->vhandle = compile(vpath, GL_VERTEX_SHADER);
	self->fhandle = compile(fpath, GL_FRAGMENT_SHADER);
	self->handle = glCreateProgram();

	glAttachShader(self->handle, self->vhandle);
	glAttachShader(self->handle, self->fhandle);

	glLinkProgram(self->handle);
}

void shader_destroy(Shader* self) {
	glDeleteProgram(self->handle);
}
