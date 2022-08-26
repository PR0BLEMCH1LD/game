#include "state.h"
#include "gfx/window.h"
#include "gfx/shader.h"
#include "gfx/atlas.h"
#include "world/world.h"

State state;

static void init(void) {
	shader_init(&state.shader, "res/shaders/default.vert", "res/shaders/default.frag");
    atlas_init(&state.atlas, "res/textures/atlas.png");
	world_init(&state.world);
    block_init();

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

static void tick(void) {
    world_set_center(&state.world, VEC3S2I(state.world.player.camera.position));

    world_tick(&state.world);
}

static void update(void) {
	world_update(&state.world);
}

static void render(void) {
	glClearColor(0.05f, 0.0f, 0.10f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	world_render(&state.world);
}

static void destroy(void) {
	shader_destroy(&state.shader);
    atlas_destroy(&state.atlas);
	world_destroy(&state.world);
}

int main() {
	window_init(init, tick, update, render, destroy);
	window_loop();

    return 0;
}
