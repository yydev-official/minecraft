#include "clouds.hpp"

namespace mcxx::classes {
    void cloud_renderer::generate(const glm::vec3 &player_pos)  {
        const siv::PerlinNoise::seed_type seed = 1337;
        const siv::PerlinNoise perlin{seed};

        std::vector<float> vertices;

        float cloud_y = 130.0f;
        int render_radius = 64; // Distance view range around player
        float scale = 0.1f;     // Balanced scale: smaller than massive, larger than tiny tufts

        int center_x = static_cast<int>(player_pos.x);
        int center_z = static_cast<int>(player_pos.z);

        // Generate in a circular radius so there are no square grid corners visible
        for (int x = -render_radius; x <= render_radius; ++x)
        {
            for (int z = -render_radius; z <= render_radius; ++z)
            {
                // Circular distance check eliminates square box corners
                if (x * x + z * z > render_radius * render_radius)
                    continue;

                int world_x = center_x + x;
                int world_z = center_z + z;

                double noise_val = perlin.octave2D_01(world_x * scale, world_z * scale, 3);

                // Inverted space check for sweeping cloud fields with gaps
                if (noise_val < 0.52f)
                {
                    float size = 1.0f;
                    float x0 = static_cast<float>(world_x);
                    float z0 = static_cast<float>(world_z);
                    float x1 = x0 + size;
                    float z1 = z0 + size;

                    float quad[] = {
                        // Top face
                        x0, cloud_y, z1,
                        x1, cloud_y, z1,
                        x1, cloud_y, z0,

                        x0, cloud_y, z1,
                        x1, cloud_y, z0,
                        x0, cloud_y, z0,

                        // Bottom face
                        x0, cloud_y, z0,
                        x1, cloud_y, z0,
                        x1, cloud_y, z1,

                        x0, cloud_y, z0,
                        x1, cloud_y, z1,
                        x0, cloud_y, z1};

                    vertices.insert(vertices.end(), std::begin(quad), std::end(quad));
                }
            }
        }

        vertex_count = static_cast<int>(vertices.size() / 3);

        if (vao == 0)
        {
            glGenVertexArrays(1, &vao);
            glGenBuffers(1, &vbo);
        }

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glBindVertexArray(0);
    }

    void cloud_renderer::render() const {
        if (vertex_count == 0) return;
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertex_count);
        glBindVertexArray(0);
    }
}