#include "player.h"
#include "../state.h"
#include "../world/world.h"

void player_init(Player* self) {
	memset(self, 0, sizeof(Player));
	camera_init(&self->camera);
    self->selected_block = END_STONE;
}

void player_tick(Player* self) {
	vec3s movement, forward, right;

	movement = GLMS_VEC3_ZERO;
	forward = (vec3s) {{ cosf(self->camera.yaw), 0.0f, sinf(self->camera.yaw) }};
	right = glms_vec3_cross(forward, (vec3s) {{ 0.0f, 1.0f, 0.0f }});

	if (window.keyboard.keys[GLFW_KEY_W].down) {
		movement = glms_vec3_add(movement, forward);
	}

	if (window.keyboard.keys[GLFW_KEY_S].down) {
		movement = glms_vec3_sub(movement, forward);
	}

	if (window.keyboard.keys[GLFW_KEY_D].down) {
		movement = glms_vec3_add(movement, right);
	}

	if (window.keyboard.keys[GLFW_KEY_A].down) {
		movement = glms_vec3_sub(movement, right);
	}

	if (window.keyboard.keys[GLFW_KEY_SPACE].down) {
		movement = glms_vec3_add(movement, (vec3s) {{ 0.0f, 1.0f, 0.0f }});
	}

	if (window.keyboard.keys[GLFW_KEY_LEFT_SHIFT].down) {
		movement = glms_vec3_sub(movement, (vec3s) {{ 0.0f, 1.0f, 0.0f }});
	}

	movement = glms_vec3_normalize(movement);
	movement = glms_vec3_scale(movement, self->speed / window.tickrate);

	self->camera.position = glms_vec3_add(self->camera.position, movement);

    ivec3s ipos;
    Direction iface;

    if (ray_intersect_block((Ray) { .origin = self->camera.position, .direction = self->camera.direction }, &ipos, &iface)) {
        if (window.mouse.buttons[GLFW_MOUSE_BUTTON_LEFT].pressed) {
            world_set_data(&state.world, ipos, AIR);

            world_append_modified_data(&state.world, ((WorldModifiedData) { .position = ipos, .data = AIR }));
        }

        if (window.mouse.buttons[GLFW_MOUSE_BUTTON_RIGHT].pressed && world_get_data(&state.world, VEC3S2I(self->camera.position)) == AIR) {
            ivec3s dpos = glms_ivec3_add(ipos, DIR2IVEC3S(iface));

            world_set_data(&state.world, dpos, self->selected_block);

            world_append_modified_data(&state.world, ((WorldModifiedData) { .position = dpos, .data = self->selected_block }));
        }
    }
}

void player_update(Player* self) {
    self->camera.yaw += window.mouse.delta.x / (((f32)window.frame_delta) / (window.mouse.sensitivity * 10000.0f));
	self->camera.pitch -= window.mouse.delta.y / (((f32)window.frame_delta) / (window.mouse.sensitivity * 10000.0f));

    camera_update(&self->camera);

    for (u8 idx = 0; idx < 5; idx++) {
        if (window.keyboard.keys[GLFW_KEY_1 + idx].pressed) {
            self->selected_block = (BlockId[5]) {
                END_STONE,
                END_STONE_BRICKS,
                OBSIDIAN,
                BEDROCK,
                PURPUR_BLOCK
            }[idx];
        }
    }
}
