#include "block.h"

Block BLOCKS[BLOCKID_LAST + 1];

void end_stone_init(void);
void end_stone_bricks_init(void);
void obsidian_init(void);
void bedrock_init(void);
void purpur_block_init(void);

void block_init(void) {
    end_stone_init();    
    end_stone_bricks_init();
    obsidian_init();
    bedrock_init();
    purpur_block_init();
}
