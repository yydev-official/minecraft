#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <vector>

#include <PerlinNoise/PerlinNoise.hpp>

namespace mcxx::classes {
    class cloud_renderer {
        public:
            unsigned int vao = 0, vbo = 0;
            int vertex_count = 0;

            void generate(const glm::vec3& player_pos);

            void render() const;
        };
}