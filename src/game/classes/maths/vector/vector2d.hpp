#pragma once

namespace mcxx::classes::maths {
    struct vector_2d {
        int x_pos = 0;
        int y_pos = 0;

        vector_2d() : x_pos(0), y_pos(0) {};

        
        
        vector_2d(int x_pos, int y_pos) : x_pos(x_pos), y_pos(y_pos) {};
        vector_2d(const vector_2d& other) : x_pos(other.x_pos), y_pos(other.y_pos) {};
        vector_2d(float x_pos, float y_pos) : 
            x_pos(static_cast<int>(x_pos)), 
            y_pos(static_cast<int>(y_pos)) {};

        // Increment operator
        void operator++() {
            x_pos++;
            y_pos++;
        }

        // Addition operator
        vector_2d operator+(const vector_2d& other) const {
            return {x_pos + other.x_pos, y_pos + other.y_pos};
        }

        // Subtraction operator
        vector_2d operator-(const vector_2d& other) const {
            return {x_pos - other.x_pos, y_pos - other.y_pos};
        }

        // Multiplication operator (scaling by an integer)
        vector_2d operator*(int scalar) const {
            return {x_pos * scalar, y_pos * scalar};
        }

        // Multiplication operator (scaling by another vector3d)
        vector_2d operator*(const vector_2d& scalar) const {
            return {x_pos * scalar.x_pos, y_pos * scalar.y_pos};
        }
    };
}