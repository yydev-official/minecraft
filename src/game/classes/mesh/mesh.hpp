#pragma once
#include <glad/glad.h>
#include <vector>

#include "game/classes/chunk/chunk.hpp"

struct mesh_data {
    unsigned int vao = 0, vbo = 0;
    int vertex_count = 0;
};

class chunk_mesh_builder {
public:
    static mesh_data build(const chunk& c);
};