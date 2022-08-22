#ifndef BLOCK_H
#define BLOCK_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#define BLOCK_ID_SIZE 6

typedef enum BlockId {
    AIR = 0,
    END_STONE,
    END_STONE_BRICKS,
    OBSIDIAN,
    BEDROCK,
    PURPUR_BLOCK
} BlockId;

typedef struct Block {
    ivec2s texture_location;
} Block;

extern Block BLOCKS[BLOCK_ID_SIZE];

void block_init(void);

#endif
