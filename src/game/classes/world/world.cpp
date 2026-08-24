#include "world.hpp"

void world::update(const glm::vec3& player_pos) {
    int player_chunk_x = static_cast<int>(std::floor(player_pos.x / CHUNK_WIDTH));
    int player_chunk_z = static_cast<int>(std::floor(player_pos.z / CHUNK_LENGTH));

    for (int x = player_chunk_x - render_distance; x <= player_chunk_x + render_distance; x++) {
        for (int z = player_chunk_z - render_distance; z <= player_chunk_z + render_distance; z++) {
            chunk_coord coord = {x, z};

            if (chunks.find(coord) == chunks.end()) {
                rendered_chunk rc;
                rc.data = std::make_unique<chunk>();

                int world_x = coord.x * CHUNK_WIDTH;
                int world_z = coord.z * CHUNK_LENGTH;
                rc.data->generate_terrain(world_x, world_z);
                rc.mesh = chunk_mesh_builder::build(*rc.data);

                chunks[coord] = std::move(rc);
            }
        }
    }

}

void world::set_block_at(int x, int y, int z, block_id id) {
    int c_x = x >= 0 ? x / CHUNK_WIDTH : (x - CHUNK_WIDTH + 1) / CHUNK_WIDTH;
    int c_z = z >= 0 ? z / CHUNK_LENGTH : (z - CHUNK_LENGTH + 1) / CHUNK_LENGTH;

    chunk_coord coord = {c_x, c_z};
    auto it = chunks.find(coord);
    if (it != chunks.end()) {
        int local_x = x - (c_x * CHUNK_WIDTH);
        int local_z = z - (c_z * CHUNK_LENGTH);
        if (local_x < 0) local_x += CHUNK_WIDTH;
        if (local_z < 0) local_z += CHUNK_LENGTH;

        if (y >= 0 && y < CHUNK_HEIGHT) {
            it->second.data->set_block(local_x, y, local_z, id);
            it->second.mesh = chunk_mesh_builder::build(*it->second.data);
        }
    }
}

const std::unordered_map<chunk_coord, rendered_chunk, chunk_hash>& world::get_chunks() const {
    return chunks;
}