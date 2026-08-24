#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

class ui_renderer {
    private:
        unsigned int hotbar_texture = 0;
        unsigned int selector_texture = 0;

        unsigned int compile_shader(const char* vs_code, const char* fs_code);

    public:
        int active_slot = 0; // Ranges from 0 to 8

        void init();
        void render(unsigned int screen_width, unsigned int screen_height);
        void set_active_slot(int slot);
};