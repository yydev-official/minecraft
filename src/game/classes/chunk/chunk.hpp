#pragma once
#include <vector>
#include <cstdint>
#include <PerlinNoise/PerlinNoise.hpp>

using block_id = uint16_t;

const int CHUNK_WIDTH = 16;
const int CHUNK_HEIGHT = 256;
const int CHUNK_LENGTH = 16;

class chunk {
private:
    std::vector<block_id> blocks;

public:
    chunk() {
        blocks.resize(CHUNK_WIDTH * CHUNK_HEIGHT * CHUNK_LENGTH, 0); // 0 = Air
        generate_terrain();
    }

    void set_block(int x, int y, int z, block_id id);
    block_id get_block(int x, int y, int z) const;

    void generate_terrain(int world_offset_x = 0, int world_offset_z = 0);
};