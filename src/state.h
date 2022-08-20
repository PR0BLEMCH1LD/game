#ifndef STATE_H
#define STATE_H

#include "gfx/gfx.h"
#include "util/util.h"

#include "gfx/window.h"
#include "gfx/shader.h"
#include "gfx/texture.h"
#include "world/world.h"

typedef struct State {
	Window* window;
	Shader shader;
	Texture texture;
	World world;
} State;

extern State state;

#endif
