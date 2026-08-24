#pragma once
#include <unordered_map>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

#include "game/classes/chunk/chunk.hpp"
#include "game/classes/mesh/mesh.hpp"

struct chunk_coord {
    int x;
    int z;

    bool operator==(const chunk_coord& other) const {
        return x == other.x && z == other.z;
    }
};

struct chunk_hash {
    template <class T1, class T2>
    std::size_t operator()(const std::pair<T1, T2>& p) const {
        return std::hash<T1>{}(p.first) ^ (std::hash<T2>{}(p.second) << 1);
    }
    std::size_t operator()(const chunk_coord& coord) const {
        return std::hash<int>{}(coord.x) ^ (std::hash<int>{}(coord.z) << 1);
    }
};

struct rendered_chunk {
    std::unique_ptr<chunk> data;
    mesh_data mesh;
};

class world {
    private:
        std::unordered_map<chunk_coord, rendered_chunk, chunk_hash> chunks;
        int render_distance = 4;

    public:
        void update(const glm::vec3& player_pos);

        void set_block_at(int x, int y, int z, block_id id);

        const std::unordered_map<chunk_coord, rendered_chunk, chunk_hash>& get_chunks() const;
};