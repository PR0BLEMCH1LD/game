#ifndef PLAYER_H
#define PLAYER_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#include "camera.h"

typedef struct Player {
	Camera camera;
	f32 speed, reach;
} Player;

void player_init(Player* self);
void player_tick(Player* self);
void player_update(Player* self);

#endif
