#include "mesh.hpp"

mesh_data chunk_mesh_builder::build(const chunk& c) {
    std::vector<float> vertices;

    for (int x = 0; x < CHUNK_WIDTH; x++) {
        for (int y = 0; y < CHUNK_HEIGHT; y++) {
            for (int z = 0; z < CHUNK_LENGTH; z++) {
                block_id current_id = c.get_block(x, y, z);
                if (current_id == 0) continue; // Skip Air[cite: 1]

                // Pick colors based on block type[cite: 1]
                float r = 0.5f, g = 0.5f, b = 0.5f; // Stone default (ID 1)[cite: 1]
                if (current_id == 2) { r = 0.4f; g = 0.3f; b = 0.2f; } // Dirt (ID 2)[cite: 1]
                if (current_id == 3) { r = 0.2f; g = 0.2f; b = 0.2f; } // Bedrock (ID 3)[cite: 1]
                if (current_id == 4) { r = 0.2f; g = 0.8f; b = 0.2f; } // Grass (ID 4)[cite: 1]

                float fx = (float)x;
                float fy = (float)y;
                float fz = (float)z;

                // 1. Back Face (-Z)[cite: 1] | Normal: (0, 0, -1)
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

                // 2. Front Face (+Z)[cite: 1] | Normal: (0, 0, 1)
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

                // 3. Bottom Face (-Y)[cite: 1] | Normal: (0, -1, 0)
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

                // 4. Top Face (+Y)[cite: 1] | Normal: (0, 1, 0)
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

                // 5. Left Face (-X)[cite: 1] | Normal: (-1, 0, 0)
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

                // 6. Right Face (+X)[cite: 1] | Normal: (1, 0, 0)
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
    // Each vertex now consists of 9 floats (3 pos + 3 color + 3 normal)
    mesh.vertex_count = vertices.size() / 9;

    if (mesh.vertex_count > 0) {
        glGenVertexArrays(1, &mesh.vao);
        glGenBuffers(1, &mesh.vbo);

        glBindVertexArray(mesh.vao);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        GLsizei stride = 9 * sizeof(float);

        // 1. Position Attribute (Location = 0)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glEnableVertexAttribArray(0);

        // 2. Color Attribute (Location = 1)
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // 3. Normal Attribute (Location = 2)
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    return mesh;
}