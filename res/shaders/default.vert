#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 v_uv;

uniform mat4 m, v, p;

void main() {
	gl_Position = p * v * m * vec4(position, 1.0f);
	v_uv = uv;
}
