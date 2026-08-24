#include "mesh.hpp"

mesh_data chunk_mesh_builder::build(const chunk& c) {
    std::vector<float> vertices;

    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_LENGTH; z++) {
                block_id current_id = c.get_block(x, y, z);
                if (current_id == 0) continue;

                float r = 0.5f, g = 0.5f, b = 0.5f;
                if (current_id == 2) { r = 0.4f; g = 0.3f; b = 0.2f; }
                if (current_id == 3) { r = 0.2f; g = 0.2f; b = 0.2f; }
                if (current_id == 4) { r = 0.2f; g = 0.8f; b = 0.2f; }

                float fx = (float)x;
                float fy = (float)y;
                float fz = (float)z;

                if (z - 1 < 0 || c.get_block(x, y, z - 1) == 0) {
                    float v[] = {
                        fx,       fy,       fz,       r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, -1.0f,
                        fx + 1.0f, fy + 1.0f, fz,       r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, -1.0f,
                        fx + 1.0f, fy,       fz,       r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, -1.0f,
                        fx,       fy,       fz,       r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, -1.0f,
                        fx,       fy + 1.0f, fz,       r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, -1.0f,
                        fx + 1.0f, fy + 1.0f, fz,       r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, -1.0f
                    };
                    vertices.insert(vertices.end(), std::begin(v), std::end(v));
                }

                if (z + 1 >= CHUNK_LENGTH || c.get_block(x, y, z + 1) == 0) {
                    float v[] = {
                        fx,       fy,       fz + 1.0f, r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, 1.0f,
                        fx + 1.0f, fy,       fz + 1.0f, r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, 1.0f,
                        fx + 1.0f, fy + 1.0f, fz + 1.0f, r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, 1.0f,
                        fx + 1.0f, fy + 1.0f, fz + 1.0f, r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, 1.0f,
                        fx,       fy + 1.0f, fz + 1.0f, r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, 1.0f,
                        fx,       fy,       fz + 1.0f, r*0.8f, g*0.8f, b*0.8f,  0.0f, 0.0f, 1.0f
                    };
                    vertices.insert(vertices.end(), std::begin(v), std::end(v));
                }

                if (y - 1 < 0 || c.get_block(x, y - 1, z) == 0) {
                    float v[] = {
                        fx,       fy, fz + 1.0f, r*0.6f, g*0.6f, b*0.6f,  0.0f, -1.0f, 0.0f,
                        fx + 1.0f, fy, fz,       r*0.6f, g*0.6f, b*0.6f,  0.0f, -1.0f, 0.0f,
                        fx + 1.0f, fy, fz + 1.0f, r*0.6f, g*0.6f, b*0.6f,  0.0f, -1.0f, 0.0f,
                        fx,       fy, fz,       r*0.6f, g*0.6f, b*0.6f,  0.0f, -1.0f, 0.0f,
                        fx + 1.0f, fy, fz,       r*0.6f, g*0.6f, b*0.6f,  0.0f, -1.0f, 0.0f,
                        fx,       fy, fz + 1.0f, r*0.6f, g*0.6f, b*0.6f,  0.0f, -1.0f, 0.0f
                    };
                    vertices.insert(vertices.end(), std::begin(v), std::end(v));
                }

                if (y + 1 >= CHUNK_HEIGHT || c.get_block(x, y + 1, z) == 0) {
                    float v[] = {
                        fx,       fy + 1.0f, fz,       r, g, b,  0.0f, 1.0f, 0.0f,
                        fx + 1.0f, fy + 1.0f, fz + 1.0f, r, g, b,  0.0f, 1.0f, 0.0f,
                        fx + 1.0f, fy + 1.0f, fz,       r, g, b,  0.0f, 1.0f, 0.0f,
                        fx,       fy + 1.0f, fz,       r, g, b,  0.0f, 1.0f, 0.0f,
                        fx,       fy + 1.0f, fz + 1.0f, r, g, b,  0.0f, 1.0f, 0.0f,
                        fx + 1.0f, fy + 1.0f, fz + 1.0f, r, g, b,  0.0f, 1.0f, 0.0f
                    };
                    vertices.insert(vertices.end(), std::begin(v), std::end(v));
                }

                if (x - 1 < 0 || c.get_block(x - 1, y, z) == 0) {
                    float v[] = {
                        fx, fy,       fz + 1.0f, r*0.7f, g*0.7f, b*0.7f,  -1.0f, 0.0f, 0.0f,
                        fx, fy + 1.0f, fz,       r*0.7f, g*0.7f, b*0.7f,  -1.0f, 0.0f, 0.0f,
                        fx, fy,       fz,       r*0.7f, g*0.7f, b*0.7f,  -1.0f, 0.0f, 0.0f,
                        fx, fy + 1.0f, fz,       r*0.7f, g*0.7f, b*0.7f,  -1.0f, 0.0f, 0.0f,
                        fx, fy,       fz + 1.0f, r*0.7f, g*0.7f, b*0.7f,  -1.0f, 0.0f, 0.0f,
                        fx, fy + 1.0f, fz + 1.0f, r*0.7f, g*0.7f, b*0.7f,  -1.0f, 0.0f, 0.0f
                    };
                    vertices.insert(vertices.end(), std::begin(v), std::end(v));
                }

                if (x + 1 >= CHUNK_WIDTH || c.get_block(x + 1, y, z) == 0) {
                    float v[] = {
                        fx + 1.0f, fy,       fz,       r*0.7f, g*0.7f, b*0.7f,  1.0f, 0.0f, 0.0f,
                        fx + 1.0f, fy + 1.0f, fz + 1.0f, r*0.7f, g*0.7f, b*0.7f,  1.0f, 0.0f, 0.0f,
                        fx + 1.0f, fy,       fz + 1.0f, r*0.7f, g*0.7f, b*0.7f,  1.0f, 0.0f, 0.0f,
                        fx + 1.0f, fy + 1.0f, fz + 1.0f, r*0.7f, g*0.7f, b*0.7f,  1.0f, 0.0f, 0.0f,
                        fx + 1.0f, fy,       fz,       r*0.7f, g*0.7f, b*0.7f,  1.0f, 0.0f, 0.0f,
                        fx + 1.0f, fy + 1.0f, fz,       r*0.7f, g*0.7f, b*0.7f,  1.0f, 0.0f, 0.0f
                    };
                    vertices.insert(vertices.end(), std::begin(v), std::end(v));
                }
            }
        }
    }

    mesh_data mesh;
    mesh.vertex_count = vertices.size() / 9;

    if (mesh.vertex_count > 0) {
        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        GLsizei stride = 9 * sizeof(float);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    return mesh;
}