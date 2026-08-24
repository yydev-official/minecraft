#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <string>

#include "classes/player/player.hpp"
#include "classes/world/world.hpp"
#include "classes/world/clouds.hpp"
#include "classes/physics/physics.hpp"
#include "classes/raycast/raycast.hpp"
#include "classes/ui/ui.hpp"

#include "utility/shader.hpp"
#include "utility/shader_program.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "utility/texture.hpp"

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
float fog_density = 0.5f;

player cam;

float last_x = SCR_WIDTH / 2.0f;
float last_y = SCR_HEIGHT / 2.0f;
bool first_mouse = true;
float delta_time = 0.0f;
float last_frame = 0.0f;

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (first_mouse) {
        last_x = static_cast<float>(xpos);
        last_y = static_cast<float>(ypos);
        first_mouse = false;
    }
    float xoffset = static_cast<float>(xpos) - last_x;
    float yoffset = last_y - static_cast<float>(ypos);
    last_x = static_cast<float>(xpos);
    last_y = static_cast<float>(ypos);
    cam.process_mouse(xoffset, yoffset);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

bool intersects_player(const glm::ivec3& block_pos, const player& player) {
    aabb player_box = aabb::from_player_position(player.position, player.player_height, player.player_radius);
    aabb block_box = { glm::vec3(block_pos), glm::vec3(block_pos) + glm::vec3(1.0f) };
    return (player_box.min.x < block_box.max.x && player_box.max.x > block_box.min.x) &&
           (player_box.min.y < block_box.max.y && player_box.max.y > block_box.min.y) &&
           (player_box.min.z < block_box.max.z && player_box.max.z > block_box.min.z);
}

int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Minec++raft - Shadows", NULL, NULL);
    if (!window) { glfwTerminate(); return -1; }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) return -1;

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    // ==========================================
    // SHADERS SETUP USING WRAPPER
    // ==========================================
    std::string depth_vs = load_shader_source("src/shaders/depth/depth.vs");
    std::string depth_fs = load_shader_source("src/shaders/depth/depth.fs");

    shader_program depth_shader;
    depth_shader.load_from_source(depth_vs.c_str(), depth_fs.c_str());

    std::string chunk_vs = load_shader_source("src/shaders/chunk/chunk.vs");
    std::string chunk_fs = load_shader_source("src/shaders/chunk/chunk.fs");

    shader_program chunk_shader;
    chunk_shader.load_from_source(chunk_vs.c_str(), chunk_fs.c_str());

    // Sun Shader Source
    std::string sun_vs = load_shader_source("src/shaders/sun/sun.vs");
    std::string sun_fs = load_shader_source("src/shaders/sun/sun.fs");

    shader_program sun_shader;
    sun_shader.load_from_source(sun_vs.c_str(), sun_fs.c_str());

    // Load Sun Texture
    unsigned int sun_texture = load_texture_ui("assets/sprites/sun.png");

    // Clouds yay!
    std::string cloud_vs = load_shader_source("src/shaders/clouds/clouds.vs");
    std::string cloud_fs = load_shader_source("src/shaders/clouds/clouds.fs");

    shader_program cloud_shader;
    cloud_shader.load_from_source(cloud_vs, cloud_fs);

    // Hardcode the highlight shader because PEOPLE BETTER NOT MESS WITH THIS SHIT
    std::string line_vs = "#version 330 core\n"
        "layout (location = 0) in vec3 a_pos;\n"
        "uniform mat4 model;\n"
        "uniform mat4 view;\n"
        "uniform mat4 projection;\n"
        "void main() {\n"
        "   gl_Position = projection * view * model * vec4(a_pos, 1.0);\n"
        "}\0";

    std::string line_fs = "#version 330 core\n"
        "out vec4 frag_color;\n"
        "void main() {\n"
        "   frag_color = vec4(0.0f, 0.0f, 0.0f, 1.0f); // Black outline/crosshair color\n"
        "}\0";

    shader_program line_shader;
    line_shader.load_from_source(line_vs, line_fs);

    // ==========================================
    // FRAMEBUFFER SETUP FOR SHADOW MAPPING
    // ==========================================
    unsigned int depth_map_fbo;
    glGenFramebuffers(1, &depth_map_fbo);

    unsigned int depth_map;
    glGenTextures(1, &depth_map);
    glBindTexture(GL_TEXTURE_2D, depth_map);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float border_color[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border_color);

    glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depth_map, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ==========================================
    // SETUP VAOs (Wireframe, Crosshair, Sun)
    // ==========================================
    float wire_cube_vertices[] = {
        0.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,  1.0f,0.0f,0.0f,  1.0f,1.0f,0.0f,
        1.0f,1.0f,0.0f,  0.0f,1.0f,0.0f,  0.0f,1.0f,0.0f,  0.0f,0.0f,0.0f,
        0.0f,0.0f,1.0f,  1.0f,0.0f,1.0f,  1.0f,0.0f,1.0f,  1.0f,1.0f,1.0f,
        1.0f,1.0f,1.0f,  0.0f,1.0f,1.0f,  0.0f,1.0f,1.0f,  0.0f,0.0f,1.0f,
        0.0f,0.0f,0.0f,  0.0f,0.0f,1.0f,  1.0f,0.0f,0.0f,  1.0f,0.0f,1.0f,
        1.0f,1.0f,0.0f,  1.0f,1.0f,1.0f,  0.0f,1.0f,0.0f,  0.0f,1.0f,1.0f
    };
    unsigned int wire_vao, wire_vbo;
    glGenVertexArrays(1, &wire_vao);
    glGenBuffers(1, &wire_vbo);
    glBindVertexArray(wire_vao);
    glBindBuffer(GL_ARRAY_BUFFER, wire_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(wire_cube_vertices), wire_cube_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float crosshair_vertices[] = { -10.0f, 0.0f, 10.0f, 0.0f, 0.0f, -10.0f, 0.0f, 10.0f };
    unsigned int cross_vao, cross_vbo;
    glGenVertexArrays(1, &cross_vao);
    glGenBuffers(1, &cross_vbo);
    glBindVertexArray(cross_vao);
    glBindBuffer(GL_ARRAY_BUFFER, cross_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(crosshair_vertices), crosshair_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float sun_vertices[] = {
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,   0.0f, 0.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f,   0.0f, 1.0f,
         1.0f, -1.0f, 0.0f,   1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f
    };
    unsigned int sun_vao, sun_vbo;
    glGenVertexArrays(1, &sun_vao);
    glGenBuffers(1, &sun_vbo);
    glBindVertexArray(sun_vao);
    glBindBuffer(GL_ARRAY_BUFFER, sun_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sun_vertices), sun_vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);

    // ==========================================
    // INITIALIZE UI
    // ==========================================
    ui_renderer game_ui;
    game_ui.init();

    cloud_renderer clouds;
    clouds.generate(cam.position);

    world my_world;

    bool left_pressed_last = false;
    bool right_pressed_last = false;

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        float current_frame = static_cast<float>(glfwGetTime());
        delta_time = current_frame - last_frame;
        last_frame = current_frame;

        cam.handle_input(window, delta_time, my_world);

        my_world.update(cam.position);
        clouds.generate(cam.position);

        glm::mat4 projection = glm::perspective(
            glm::radians(100.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT,
            0.1f,
            100.0f
        );

        raycast_result hit = raycaster::cast(cam.position, cam.front, 4.0f, my_world);
        bool left_pressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
        bool right_pressed = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, true);
        }

        for (int i = 0; i < 9; ++i) {
            if (glfwGetKey(window, GLFW_KEY_1 + i) == GLFW_PRESS) {
                game_ui.set_active_slot(i);
            }
        }

        if (hit.hit) {
            if (left_pressed && !left_pressed_last) {
                my_world.set_block_at(hit.block_pos.x, hit.block_pos.y, hit.block_pos.z, 0);
            } else if (right_pressed && !right_pressed_last) {
                if (!intersects_player(hit.adjacent_pos, cam)) {
                    my_world.set_block_at(hit.adjacent_pos.x, hit.adjacent_pos.y, hit.adjacent_pos.z, 1);
                }
            }
        }
        left_pressed_last = left_pressed;
        right_pressed_last = right_pressed;

        glm::vec3 light_dir = glm::normalize(glm::vec3(-0.4f, -1.0f, -0.3f));
        glm::vec3 light_pos = cam.position - light_dir * 50.0f;
        glm::mat4 light_projection = glm::ortho(-60.0f, 60.0f, -60.0f, 60.0f, 1.0f, 150.0f);
        glm::mat4 light_view = glm::lookAt(light_pos, cam.position, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 light_space_matrix = light_projection * light_view;

        // ==========================================
        // PASS 1: RENDER TO SHADOW MAP
        // ==========================================
        glBindFramebuffer(GL_FRAMEBUFFER, depth_map_fbo);
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glClear(GL_DEPTH_BUFFER_BIT);
        depth_shader.use();
        depth_shader.set_mat4("light_space_matrix", light_space_matrix);

        for (const auto& [coord, chunk_obj] : my_world.get_chunks()) {
            if (chunk_obj.mesh.vertex_count == 0) continue;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * CHUNK_WIDTH, 0.0f, coord.z * CHUNK_LENGTH));
            depth_shader.set_mat4("model", model);
            glBindVertexArray(chunk_obj.mesh.vao);
            glDrawArrays(GL_TRIANGLES, 0, chunk_obj.mesh.vertex_count);
        }

        // ==========================================
        // PASS 2: RENDER SCENE WITH SHADOWS
        // ==========================================
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        glClearColor(0.53f, 0.81f, 0.92f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        chunk_shader.use();
        glm::mat4 view = cam.get_view_matrix();
        chunk_shader.set_mat4("view", view);
        chunk_shader.set_mat4("projection", projection);
        chunk_shader.set_mat4("light_space_matrix", light_space_matrix);
        chunk_shader.set_vec3("light_dir", light_dir);
        chunk_shader.set_vec3("view_pos", cam.position);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depth_map);
        chunk_shader.set_int("shadow_map", 0);

        for (const auto& [coord, chunk_obj] : my_world.get_chunks()) {
            if (chunk_obj.mesh.vertex_count == 0) continue;
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(coord.x * CHUNK_WIDTH, 0.0f, coord.z * CHUNK_LENGTH));
            chunk_shader.set_mat4("model", model);
            glBindVertexArray(chunk_obj.mesh.vao);
            glDrawArrays(GL_TRIANGLES, 0, chunk_obj.mesh.vertex_count);
        }

        // ==========================================
        // PASS 3: RENDER SUN
        // ==========================================
        sun_shader.use();
        sun_shader.set_mat4("projection", projection);
        sun_shader.set_mat4("view", view);

        // Position the sun far away in the sky direction
        glm::vec3 sun_world_pos = cam.position - light_dir * 400.0f;

        // Build a stable model matrix that faces the camera without tracking rotation weirdly,
        // or simply align it perpendicular to the light direction vector.
        glm::mat4 sun_model = glm::mat4(1.0f);
        sun_model = glm::translate(sun_model, sun_world_pos);

        // Face the sun flatly toward the camera's position cleanly
        glm::vec3 to_camera = glm::normalize(cam.position - sun_world_pos);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::normalize(glm::cross(up, to_camera));
        up = glm::cross(to_camera, right);

        sun_model[0] = glm::vec4(right, 0.0f);
        sun_model[1] = glm::vec4(up, 0.0f);
        sun_model[2] = glm::vec4(to_camera, 0.0f);

        sun_model = glm::scale(sun_model, glm::vec3(45.0f));
        sun_shader.set_mat4("model", sun_model);

        glDisable(GL_CULL_FACE); // Ensure backface culling doesn't hide the quad
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, sun_texture);
        sun_shader.set_int("sunTexture", 0);

        glBindVertexArray(sun_vao);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE); // Re-enable culling for terrain

        // ==========================================
        // PASS 3.5: RENDER PERLIN NOISE CLOUDS
        // ==========================================
        cloud_shader.use();
        cloud_shader.set_mat4("projection", projection);
        cloud_shader.set_mat4("view", view);
        cloud_shader.set_mat4("model", glm::mat4(1.0f));
        cloud_shader.set_vec3("view_pos", cam.position);

        glDisable(GL_CULL_FACE);
        glDepthMask(GL_FALSE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        clouds.render();

        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);
        glEnable(GL_CULL_FACE);

        // ==========================================
        // PASS 4: RENDER BLOCK HIGHLIGHT WIREFRAME
        // ==========================================
        if (hit.hit) {
            line_shader.use(); // Use the dedicated line shader
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(hit.block_pos));
            model = glm::scale(model, glm::vec3(1.002f));
            line_shader.set_mat4("model", model);
            line_shader.set_mat4("view", view);
            line_shader.set_mat4("projection", projection);

            glLineWidth(2.0f);
            glBindVertexArray(wire_vao);
            glDrawArrays(GL_LINES, 0, 24);
            glBindVertexArray(0);
        }

        // ==========================================
        // PASS 5: RENDER 2D CROSSHAIR
        // ==========================================
        glDisable(GL_DEPTH_TEST);
        glm::mat4 ortho = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);
        glm::mat4 cross_model = glm::translate(glm::mat4(1.0f), glm::vec3(SCR_WIDTH / 2.0f, SCR_HEIGHT / 2.0f, 0.0f));

        line_shader.use(); // Use the dedicated line shader
        line_shader.set_mat4("projection", ortho);
        line_shader.set_mat4("view", glm::mat4(1.0f));
        line_shader.set_mat4("model", cross_model);

        glLineWidth(2.0f);
        glBindVertexArray(cross_vao);
        glDrawArrays(GL_LINES, 0, 4);
        glBindVertexArray(0);
        glEnable(GL_DEPTH_TEST);

        // ==========================================
        // PASS 6: RENDER UI / HOTBAR
        // ==========================================
        game_ui.render(SCR_WIDTH, SCR_HEIGHT);

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}