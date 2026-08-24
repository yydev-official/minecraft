#include "player.hpp"
#include <iostream>

namespace mcxx::classes::entities {
    player::player() : entity(identifier(1), maths::vector_3d(8, 85, 16), 20.0f) {}

    glm::mat4 player::get_view_matrix() const {
        return glm::lookAt(position, position + front, up);
    }

    void player::handle_input(GLFWwindow* window, float deltaTime, const world& my_world) {
        if (deltaTime > 0.1f) return;

        bool ctrl_pressed = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS);

        if (!is_flying) {
            is_crouching = ctrl_pressed;
        } else {
            is_crouching = false;
        }

        float target_height = is_crouching ? crouch_height : normal_height;
        player_height += (target_height - player_height) * (deltaTime * 15.0f);

        float current_speed = walkSpeed;
        if (is_crouching) {
            current_speed *= 0.5f;
        } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            current_speed *= sprintMultiplier;
        }

        glm::vec3 flat_front = glm::normalize(glm::vec3(front.x, 0.0f, front.z));
        glm::vec3 right = glm::normalize(glm::cross(flat_front, up));

        glm::vec3 movement_dir(0.0f);
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) movement_dir += flat_front;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) movement_dir -= flat_front;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) movement_dir -= right;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) movement_dir += right;

        if (glm::length(movement_dir) > 0.0f) {
            movement_dir = glm::normalize(movement_dir);
            glm::vec3 velocity = movement_dir * current_speed * deltaTime;

            // Test X movement[cite: 1]
            position.x += velocity.x;
            if (physics::check_collision(aabb::from_player_position(position, player_height, player_radius), my_world)) {
                position.x -= velocity.x;
            }

            // Test Z movement[cite: 1]
            position.z += velocity.z;
            if (physics::check_collision(aabb::from_player_position(position, player_height, player_radius), my_world)) {
                position.z -= velocity.z;
            }
        }

        bool space_pressed = (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
        static float last_toggle_time = 0.0f;

        if (space_pressed && !space_was_pressed) {
            float current_time = static_cast<float>(glfwGetTime());
            if (current_time - last_space_press_time < 0.25f && current_time - last_toggle_time > 0.5f) {
                is_flying = !is_flying;
                velocity_y = 0.0f;
                last_toggle_time = current_time;
            }
            last_space_press_time = current_time;
        }

        if (is_flying) {
            float fly_speed = current_speed * 2.0f;
            if (space_pressed) position.y += fly_speed * deltaTime;
            if (ctrl_pressed) position.y -= fly_speed * deltaTime;
        } else {
            // Walking physics & Gravity[cite: 1]
            if (is_grounded && space_pressed && !space_was_pressed) {
                velocity_y = jump_force;
                is_grounded = false;
            }

            velocity_y += gravity * deltaTime;
            position.y += velocity_y * deltaTime;

            // Downward raycast check for landing[cite: 1]
            glm::vec3 ray_origin = position - glm::vec3(0.0f, player_height, 0.0f);
            raycast::raycast_result ground_check = raycast::raycaster::cast(ray_origin, glm::vec3(0.0f, -1.0f, 0.0f), 0.2f, my_world);

            if (ground_check.hit) {
                is_grounded = true;
                velocity_y = 0.0f;
                position.y = static_cast<float>(ground_check.block_pos.y + 1) + player_height;
            } else {
                is_grounded = false;
            }
        }

        space_was_pressed = space_pressed;

        // Sync base entity integer position block tracker with exact physics coordinates
        position_block = maths::vector_3d(static_cast<int>(position.x), static_cast<int>(position.y), static_cast<int>(position.z));
    }

    void player::process_mouse(float xoffset, float yoffset) {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;

        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(direction);
    }

    void player::update() {
        // Override base entity update loop if needed
    }

    void player::render() {
        // Render entity model if applicable
    }
}