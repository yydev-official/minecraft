#include "physics.hpp"

bool physics::is_block_solid(int block_x, int block_y, int block_z, const world& my_world) {
    int c_x = block_x >= 0 ? block_x / CHUNK_WIDTH : (block_x - CHUNK_WIDTH + 1) / CHUNK_WIDTH;
    int c_z = block_z >= 0 ? block_z / CHUNK_LENGTH : (block_z - CHUNK_LENGTH + 1) / CHUNK_LENGTH;

    chunk_coord coord = {c_x, c_z};
    const auto& chunks = my_world.get_chunks();
    auto it = chunks.find(coord);

    if (it != chunks.end()) {
        int local_x = block_x - (c_x * CHUNK_WIDTH);
        int local_z = block_z - (c_z * CHUNK_LENGTH);

        if (local_x < 0) local_x += CHUNK_WIDTH;
        if (local_z < 0) local_z += CHUNK_LENGTH;

        if (block_y >= 0 && block_y < CHUNK_HEIGHT) {
            return it->second.data->get_block(local_x, block_y, local_z) != 0;
        }
    }

    return false;
}

bool physics::check_collision(const aabb& box, const world& my_world) {
    int x_min = static_cast<int>(std::floor(box.min.x));
    int x_max = static_cast<int>(std::floor(box.max.x));
    int y_min = static_cast<int>(std::floor(box.min.y));
    int y_max = static_cast<int>(std::floor(box.max.y));
    int z_min = static_cast<int>(std::floor(box.min.z));
    int z_max = static_cast<int>(std::floor(box.max.z));

    for (int x = x_min; x <= x_max; x++) {
        for (int y = y_min; y <= y_max; y++) {
            for (int z = z_min; z <= z_max; z++) {
                if (is_block_solid(x, y, z, my_world)) {
                    return true;
                }
            }
        }
    }

    return false;
}

aabb aabb::from_player_position(const glm::vec3& pos, float height, float radius) {
    return {
        glm::vec3(pos.x - radius, pos.y - height, pos.z - radius),
        glm::vec3(pos.x + radius, pos.y, pos.z + radius)
    };
}