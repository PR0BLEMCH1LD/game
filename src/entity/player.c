#include "player.h"
#include "../state.h"
#include "../world/world.h"

void player_init(Player* self) {
	memset(self, 0, sizeof(Player));
	camera_init(&self->camera);
}

void player_tick(Player* self) {
	vec3s movement, forward, right;

	movement = GLMS_VEC3_ZERO;
	forward = (vec3s) {{ cosf(self->camera.yaw), 0.0f, sinf(self->camera.yaw) }};
	right = glms_vec3_cross(forward, (vec3s) {{ 0.0f, 1.0f, 0.0f }});

	if (state.window->keyboard.keys[GLFW_KEY_W].down) {
		movement = glms_vec3_add(movement, forward);
	}

	if (state.window->keyboard.keys[GLFW_KEY_S].down) {
		movement = glms_vec3_sub(movement, forward);
	}

	if (state.window->keyboard.keys[GLFW_KEY_D].down) {
		movement = glms_vec3_add(movement, right);
	}

	if (state.window->keyboard.keys[GLFW_KEY_A].down) {
		movement = glms_vec3_sub(movement, right);
	}

	if (state.window->keyboard.keys[GLFW_KEY_SPACE].down) {
		movement = glms_vec3_add(movement, (vec3s) {{ 0.0f, 1.0f, 0.0f }});
	}

	if (state.window->keyboard.keys[GLFW_KEY_LEFT_SHIFT].down) {
		movement = glms_vec3_sub(movement, (vec3s) {{ 0.0f, 1.0f, 0.0f }});
	}

	movement = glms_vec3_normalize(movement);
	movement = glms_vec3_scale(movement, self->speed / state.window->tickrate);

	self->camera.position = glms_vec3_add(self->camera.position, movement);

    ivec3s ipos;
    Direction iface;

    if (ray_intersect_block(
            (Ray) { .origin = self->camera.position, .direction = self->camera.direction },
            &ipos, &iface)) {
        if (state.window->mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].pressed) {
            world_set_data(&state.world, ipos, 0);

            world_append_modified_data(
                &state.world,
                ((WorldModifiedData) { 
                    .position = ipos,
                    .data = 0 
                }));
        }

        if (state.window->mouse.buttons[GLFW_MOUSE_BUTTON_RIGHT].pressed) {
            ivec3s dpos = glms_ivec3_add(ipos, DIR2IVEC3S(iface));

            world_set_data(&state.world, dpos, 1);

            world_append_modified_data(
                &state.world,
                ((WorldModifiedData) { 
                    .position = dpos,
                    .data = 1 
                }));
        }
    }
}

void player_update(Player* self) {
    self->camera.yaw += state.window->mouse.delta.x / (((f32)state.window->frame_delta) / (state.window->mouse.sensitivity * 10000.0f));
	self->camera.pitch -= state.window->mouse.delta.y / (((f32)state.window->frame_delta) / (state.window->mouse.sensitivity * 10000.0f));

    camera_update(&self->camera);
}
