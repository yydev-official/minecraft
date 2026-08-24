#pragma once

struct vector_3d {
    int x_pos = 0;
    int y_pos = 0;
    int z_pos = 0;

    vector_3d() : x_pos(0), y_pos(0), z_pos(0) {};

    
    
    vector_3d(int x_pos, int y_pos, int z_pos) : x_pos(x_pos), y_pos(y_pos), z_pos(z_pos) {};
    vector_3d(const vector_3d& other) : x_pos(other.x_pos), y_pos(other.y_pos), z_pos(other.z_pos) {};
    vector_3d(float x_pos, float y_pos, float z_pos) : 
        x_pos(static_cast<int>(x_pos)), 
        y_pos(static_cast<int>(y_pos)), 
        z_pos(static_cast<int>(z_pos)) {};
    
    // Increment operator
    void operator++() {
        x_pos++;
        y_pos++;
        z_pos++;
    }

    // Addition operator
    vector_3d operator+(const vector_3d& other) const {
        return {x_pos + other.x_pos, y_pos + other.y_pos, z_pos + other.z_pos};
    }

    // Subtraction operator
    vector_3d operator-(const vector_3d& other) const {
        return {x_pos - other.x_pos, y_pos - other.y_pos, z_pos - other.z_pos};
    }

    // Multiplication operator (scaling by an integer)
    vector_3d operator*(int scalar) const {
        return {x_pos * scalar, y_pos * scalar, z_pos * scalar};
    }

    // Multiplication operator (scaling by another vector3d)
    vector_3d operator*(const vector_3d& scalar) const {
        return {x_pos * scalar.x_pos, y_pos * scalar.y_pos, z_pos * scalar.z_pos};
    }

    glm::vec3 to_glm_vec3() {
        return glm::vec3(static_cast<float>(this->x_pos), static_cast<float>(this->y_pos), static_cast<float>(this->z_pos));
    }

    static vector_3d front() {
        return vector_3d(0, 0, -1);
    }

    static vector_3d upwards() {
        return vector_3d(0, 1, 0);
    }

    static vector_3d downwards() {
        return vector_3d(0, -1, 0);
    }

    static vector_3d left() {
        return vector_3d(-1, 0, 0);
    }

    static vector_3d right() {
        return vector_3d(1, 0, 0);
    }
};