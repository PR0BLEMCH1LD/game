#ifndef BLOCK_H
#define BLOCK_H

#include "../gfx/gfx.h"
#include "../util/util.h"

#define BLOCKID_LAST PURPUR_BLOCK

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

extern Block BLOCKS[BLOCKID_LAST + 1];

void block_init(void);

#endif
