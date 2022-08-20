#include "camera.h"
#include "../state.h"

void camera_init(Camera* self) {
	memset(self, 0, sizeof(Camera));
	self->fov = radians(45);
	self->aspect = (f32)state.window->size.x / state.window->size.y;
	self->znear = 0.01f;
	self->zfar = 1000.0f;
    self->pitch = 0.0f;
	self->yaw = -PI_2;

    camera_update(self);
}

void camera_update(Camera* self) {
	self->pitch = clamp(self->pitch, -PI_2 + 0.01f, PI_2 - 0.01f);
	self->yaw = (self->yaw < 0 ? TAU : 0.0f) + fmodf(self->yaw, TAU);
	self->direction = glms_vec3_normalize((vec3s) {{
		cosf(self->pitch) * cosf(self->yaw),
		sinf(self->pitch),
		cosf(self->pitch) * sinf(self->yaw)
	}});
	self->right = glms_vec3_cross(self->direction, (vec3s) {{ 0.0f, 1.0f, 0.0f }}); 
	self->up = glms_vec3_cross(self->right, self->direction);
	self->view = glms_lookat(self->position, glms_vec3_add(self->position, self->direction), self->up);
	self->projection = glms_perspective(self->fov, self->aspect, self->znear, self->zfar);
}
