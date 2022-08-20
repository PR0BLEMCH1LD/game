#ifndef CAMERA_H
#define CAMERA_H

#include "../gfx/gfx.h"
#include "../util/util.h"

typedef struct Camera {
	vec3s position, direction, right, up;
	f32 fov, aspect, znear, zfar, pitch, yaw;
    mat4s view, projection;
} Camera;

void camera_init(Camera* self);
void camera_update(Camera* self);

#endif
