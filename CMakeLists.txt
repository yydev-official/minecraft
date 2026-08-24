cmake_minimum_required(VERSION 3.20)
project(mcxx VERSION 1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# Fetch GLFW (Window & Input)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Fetch GLM (3D Math)
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG        1.0.1
)
FetchContent_MakeAvailable(glm)

# Recursively find all source files in the src/ directory
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.hpp"
)

# Target Executable with ImGui local source files & backends
add_executable(mcxx
    ${SOURCES}
    vendor/glad/src/glad.c
    vendor/imgui-1.92.9b/imgui.cpp
    vendor/imgui-1.92.9b/imgui_draw.cpp
    vendor/imgui-1.92.9b/imgui_tables.cpp
    vendor/imgui-1.92.9b/imgui_widgets.cpp
    vendor/imgui-1.92.9b/backends/imgui_impl_glfw.cpp
    vendor/imgui-1.92.9b/backends/imgui_impl_opengl3.cpp
)

target_include_directories(mcxx PRIVATE
    src
    vendor/glad/include
    vendor/PerlinNoise/include
    vendor/stb_image/include
    vendor/imgui-1.92.9b
    vendor/imgui-1.92.9b/backends
)

target_link_libraries(mcxx PRIVATE
    glfw
    glm::glm
)