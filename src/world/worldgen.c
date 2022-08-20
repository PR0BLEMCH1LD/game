#include "worldgen.h"

void worldgen_generate(Chunk* chunk) {
    for (s32 x = 0; x < 16; x++) {
		for (s32 z = 0; z < 16; z++) {
			for (s32 y = 0; y < 2; y++) {
				chunk_set_data(chunk, (ivec3s) {{ x, y, z }}, 1);
			}
		}
	}   
}
