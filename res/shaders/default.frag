#version 330 core
out vec4 FragColor;
in vec2 v_uv;

uniform sampler2D tex;

void main() {
	FragColor = texture(tex, v_uv);
}
