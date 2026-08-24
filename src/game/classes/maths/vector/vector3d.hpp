#pragma once
#include <glm/glm.hpp>
#include <cmath>


namespace mcxx::classes::maths {
    struct vector_3d {
        int x_pos = 0;
        int y_pos = 0;
        int z_pos = 0;

        vector_3d() : x_pos(0), y_pos(0), z_pos(0) {}
        vector_3d(int x, int y, int z) : x_pos(x), y_pos(y), z_pos(z) {}
        vector_3d(const vector_3d& other) : x_pos(other.x_pos), y_pos(other.y_pos), z_pos(other.z_pos) {}
        vector_3d(float x, float y, float z) : 
            x_pos(static_cast<int>(x)), 
            y_pos(static_cast<int>(y)), 
            z_pos(static_cast<int>(z)) {}

        void operator++() {
            x_pos++;
            y_pos++;
            z_pos++;
        }

        vector_3d operator+(const vector_3d& other) const {
            return {x_pos + other.x_pos, y_pos + other.y_pos, z_pos + other.z_pos};
        }

        vector_3d operator-(const vector_3d& other) const {
            return {x_pos - other.x_pos, y_pos - other.y_pos, z_pos - other.z_pos};
        }

        vector_3d operator*(int scalar) const {
            return {x_pos * scalar, y_pos * scalar, z_pos * scalar};
        }

        vector_3d operator*(const vector_3d& scalar) const {
            return {x_pos * scalar.x_pos, y_pos * scalar.y_pos, z_pos * scalar.z_pos};
        }

        glm::vec3 to_glm_vec3() const {
            return glm::vec3(static_cast<float>(x_pos), static_cast<float>(y_pos), static_cast<float>(z_pos));
        }

        static vector_3d front() { return vector_3d(0, 0, -1); }
        static vector_3d upwards() { return vector_3d(0, 1, 0); }
        static vector_3d downwards() { return vector_3d(0, -1, 0); }
        static vector_3d left() { return vector_3d(-1, 0, 0); }
        static vector_3d right() { return vector_3d(1, 0, 0); }
    };
}