#include "ui.hpp"

#include <iostream>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <game/utility/texture.hpp>


unsigned int ui_renderer::compile_shader(const char* vs_code, const char* fs_code) {
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_code, NULL);
    glCompileShader(vs);

    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_code, NULL);
    glCompileShader(fs);

    unsigned int program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

void ui_renderer::init() {
    hotbar_texture = load_texture_ui("assets/sprites/hotbar.png");
    selector_texture = load_texture_ui("assets/sprites/selector.png");
}

void ui_renderer::set_active_slot(int slot) {
    if (slot >= 0 && slot < 9) {
        active_slot = slot;
    }
}

void ui_renderer::render(unsigned int screen_width, unsigned int screen_height) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    float scale = 2.0f; // Keep your preferred scale value
    float hotbar_width = 288.0f * scale;
    float hotbar_height = 32.0f * scale;

    // Calculate slot width and selector size first so we know how much vertical overflow we have
    float slot_width = hotbar_width / 9.0f;
    float selector_size = slot_width * 1.05f;

    // Calculate how much the selector sticks out above the hotbar
    float vertical_overflow = (selector_size - hotbar_height) * 0.5f;

    // Make the ImGui window taller to accommodate the overflow, and shift its position up
    float window_height = hotbar_height + (vertical_overflow * 2.0f);
    ImVec2 window_pos = ImVec2((screen_width - hotbar_width) * 0.5f, screen_height - window_height - 30.0f);

    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(hotbar_width + 4.0f, window_height + 10.0f), ImGuiCond_Always);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration |
                                        ImGuiWindowFlags_NoResize |
                                        ImGuiWindowFlags_NoScrollbar |
                                        ImGuiWindowFlags_NoSavedSettings |
                                        ImGuiWindowFlags_NoBackground;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("CustomHotbar", nullptr, window_flags);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();

    // Push the drawing down by the vertical overflow amount so the hotbar stays at the bottom
    ImVec2 p0 = ImVec2(ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y + vertical_overflow);
    ImVec2 p1 = ImVec2(p0.x + hotbar_width, p0.y + hotbar_height);

    // Render Hotbar Background Texture
    draw_list->AddImage(
        (ImTextureID)(uintptr_t)hotbar_texture,
        p0, p1,
        ImVec2(0, 0), ImVec2(1, 1)
    );

    // Position of the active slot selector
    float active_x = p0.x + (active_slot * slot_width) - (selector_size - slot_width) * 0.5f;
    float active_y = p0.y - (selector_size - hotbar_height) * 0.5f;

    draw_list->AddImage(
        (ImTextureID)(uintptr_t)selector_texture,
        ImVec2(active_x, active_y),
        ImVec2(active_x + selector_size, active_y + selector_size),
        ImVec2(0, 0), ImVec2(1, 1)
    );

    // Invisible buttons over each slot for mouse clicks
    for (int i = 0; i < 9; ++i) {
        ImGui::SetCursorScreenPos(ImVec2(p0.x + (i * slot_width), p0.y));
        std::string label = "##slot" + std::to_string(i);
        if (ImGui::InvisibleButton(label.c_str(), ImVec2(slot_width, hotbar_height))) {
            set_active_slot(i);
        }
    }

    ImGui::End();
    ImGui::PopStyleVar();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}