#include "chunk.hpp"

void chunk::set_block(int x, int y, int z, block_id id) {
    if (x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_LENGTH) {
        blocks[x + (y * CHUNK_WIDTH) + (z * CHUNK_WIDTH * CHUNK_HEIGHT)] = id;
    }
}

block_id chunk::get_block(int x, int y, int z) const {
    if (x >= 0 && x < CHUNK_WIDTH && y >= 0 && y < CHUNK_HEIGHT && z >= 0 && z < CHUNK_LENGTH) {
        return blocks[x + (y * CHUNK_WIDTH) + (z * CHUNK_WIDTH * CHUNK_HEIGHT)];
    }
    return 0;
}

void chunk::generate_terrain(int world_offset_x, int world_offset_z) {
    siv::PerlinNoise::seed_type seed = 1337;
    siv::PerlinNoise perlin(seed);

    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int z = 0; z < CHUNK_LENGTH; z++) {
            double nx = static_cast<double>(x + world_offset_x) * 0.05;
            double nz = static_cast<double>(z + world_offset_z) * 0.05;

            double noise_val = perlin.octave2D_01(nx, nz, 4);
            int surface_height = static_cast<int>(48.0 + noise_val * 32.0);

            for (int y = 0; y < CHUNK_HEIGHT; y++) {
                if (y == 0) {
                    set_block(x, y, z, 3);
                } else if (y < surface_height - 3) {
                    set_block(x, y, z, 1);
                } else if (y < surface_height) {
                    set_block(x, y, z, 2);
                } else if (y == surface_height) {
                    set_block(x, y, z, 4);
                } else {
                    set_block(x, y, z, 0);
                }
            }
        }
    }
}