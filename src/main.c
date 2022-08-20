#include "state.h"
#include "gfx/window.h"
#include "gfx/shader.h"
#include "gfx/texture.h"
#include "world/world.h"

State state;

static void _init(void) {
	state.window = &window;
	shader_init(&state.shader, "res/shaders/default.vert", "res/shaders/default.frag");
	texture_init(&state.texture, "res/textures/stone.jpg");
	world_init(&state.world);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	state.world.player.camera.position = (vec3s) {{ 0.0f, 10.0f, 0.0f }};
	state.world.player.speed = 10.0f;
    state.world.player.reach = 6.0f;
}

static void _tick(void) {
    world_set_center(&state.world, VEC3S2I(state.world.player.camera.position));

    world_tick(&state.world);
}

static void _update(void) {
	world_update(&state.world);
}

static void _render(void) {
	glClearColor(0.05f, 0.0f, 0.10f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	world_render(&state.world);
}

static void _destroy(void) {
	shader_destroy(&state.shader);
	world_destroy(&state.world);
}

int main() {
	window_init(_init, _tick, _update, _render, _destroy);
	window_loop();

    return 0;
}
