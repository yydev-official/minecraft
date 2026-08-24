#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include "game/classes/world/world.hpp"

struct aabb {
    glm::vec3 min;
    glm::vec3 max;

    // Create an AABB from a player's eye position, height, and width radius
    static aabb from_player_position(const glm::vec3& pos, float height, float radius);
};

class physics {
    public:
        // Helper to check if a specific block coordinate is solid
        static bool is_block_solid(int block_x, int block_y, int block_z, const world& my_world);

        // Check if an AABB intersects any solid blocks in the world
        static bool check_collision(const aabb& box, const world& my_world);
};