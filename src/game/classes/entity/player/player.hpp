#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

#include <cmath>

#include "game/classes/raycast/raycast.hpp"
#include "game/classes/world/world.hpp"
#include "game/classes/physics/physics.hpp"
#include "game/classes/maths/vector/vector3d.hpp"
#include "game/classes/entity/entity.hpp"

class player : public entity {
    public:
        float yaw = -90.0f;
        float pitch = 0.0f;
        float walkSpeed = 3.0f;
        float sprintMultiplier = 2.0f;
        float mouseSensitivity = 0.1f;

        // Physics & Movement States
        bool is_flying = false;
        bool is_crouching = false;
        float velocity_y = 0.0f;
        float gravity = -20.0f;
        float jump_force = 7.0f;
        bool is_grounded = false;

        float player_height = 1.8f;
        float normal_height = 1.8f;
        float crouch_height = 1.4f;
        float player_radius = 0.3f;

        // Double-tap space tracking
        float last_space_press_time = 0.0f;
        bool space_was_pressed = false;

        glm::mat4 get_view_matrix() const;
        void handle_input(GLFWwindow* window, float deltaTime, const world& my_world);
        void process_mouse(float xoffset, float yoffset);

        void update() override;
        void render() override;
};