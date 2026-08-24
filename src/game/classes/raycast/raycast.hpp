#pragma once
#include <glm/glm.hpp>
#include <cmath>
#include "game/classes/world/world.hpp"
#include "game/classes/physics/physics.hpp"

struct raycast_result {
    bool hit = false;
    glm::ivec3 block_pos;     // Position of the hit block
    glm::ivec3 adjacent_pos;  // Position for placing a new block
    float distance = 0.0f;
};

class raycaster {
    public:
        static raycast_result cast(const glm::vec3& origin, const glm::vec3& direction, float max_distance, const world& my_world);
};