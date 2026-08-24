#pragma once

#include "game/classes/maths/vector/vector3d.hpp"
#include "game/classes/identifier/identifier.hpp"

class entity {
    public:
        identifier id;
        float health = 20.0f;

        vector_3d position;

        entity(identifier id, vector_3d pos, float health) : id(id), position(pos), health(health) {};
        ~entity();

        void set_position(vector_3d pos);
        void set_position(int x, int y, int z);

        vector_3d get_position() const;

        virtual void update();
        virtual void render();
};