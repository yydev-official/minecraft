#pragma once

#include "game/classes/maths/vector/vector3d.hpp"
#include "game/classes/identifier/identifier.hpp"

namespace mcxx::classes {
    class entity {
        public:
            identifier id;
            float health = 20.0f;
            maths::vector_3d position_block; // Integer position for chunk/block tracking

            entity(identifier id, maths::vector_3d pos, float health) : id(id), position_block(pos), health(health) {}
            virtual ~entity() = default;

            void set_position(maths::vector_3d pos) { position_block = pos; }
            void set_position(int x, int y, int z) { position_block = maths::vector_3d(x, y, z); }

            maths::vector_3d get_position() const { return position_block; }

            virtual void update() = 0;
            virtual void render() = 0;
    };
}