#ifndef WINDOW_H
#define WINDOW_H

#include "gfx.h"
#include "../util/util.h"

typedef void (*FWindow)();

typedef struct Button {
	bool down, pressed, last;
} Button;

typedef struct Mouse {
    Button buttons[GLFW_MOUSE_BUTTON_LAST];
	vec2s position, delta;
	f32 sensitivity;
    bool not_first; 
} Mouse;

typedef struct Keyboard {
	Button keys[GLFW_KEY_LAST];
} Keyboard;

typedef struct Window {
	GLFWwindow* handle;
    Keyboard keyboard;
	Mouse mouse;
    ivec2s size;
    FWindow init, tick, update, render, destroy;
	u64 last_second;
	u64 frames, fps, last_frame, frame_delta;
	u64 ticks, tps, tick_remainder;
	u32 tickrate;
} Window;

extern Window window;
	
void window_init(FWindow init, FWindow tick, FWindow update, FWindow render, FWindow destroy);
void window_loop();

#endif
