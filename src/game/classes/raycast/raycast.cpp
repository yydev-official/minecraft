#include "raycast.hpp"

namespace mcxx::classes::raycast {
    raycast_result raycaster::cast(const glm::vec3& origin, const glm::vec3& direction, float max_distance, const world& my_world) {
        raycast_result result;

        float step = 0.05f;
        float current_dist = 0.0f;
        glm::ivec3 last_voxel(-1);

        while (current_dist < max_distance) {
            glm::vec3 point = origin + direction * current_dist;
            glm::ivec3 voxel(std::floor(point.x), std::floor(point.y), std::floor(point.z));

            if (voxel != last_voxel) {
                if (physics::is_block_solid(voxel.x, voxel.y, voxel.z, my_world)) {
                    result.hit = true;
                    result.block_pos = voxel;
                    result.distance = current_dist;

                    // Calculate adjacent position by stepping backwards slightly along the ray
                    glm::vec3 prev_point = origin + direction * (current_dist - step);
                    result.adjacent_pos = glm::ivec3(std::floor(prev_point.x), std::floor(prev_point.y), std::floor(prev_point.z));
                    return result;
                }
                last_voxel = voxel;
            }
            current_dist += step;
        }

        return result;
    }
}