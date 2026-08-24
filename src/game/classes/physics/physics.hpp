#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include "game/classes/world/world.hpp"

struct aabb {
    glm::vec3 min;
    glm::vec3 max;

    static aabb from_player_position(const glm::vec3& pos, float height, float radius);
};

class physics {
    public:
        static bool is_block_solid(int block_x, int block_y, int block_z, const world& my_world);

        static bool check_collision(const aabb& box, const world& my_world);
};