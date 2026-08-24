#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "game/classes/identifier/identifier.hpp"

namespace mcxx::classes::items {
    using attribute_value = std::variant<int, float, classes::identifier, bool>;

    struct item_definition {
        classes::identifier id;
        classes::identifier name;
        std::unordered_map<classes::identifier, attribute_value> attributes;

        template<typename T>
        T get_attribute(const classes::identifier& key, T defaultValue) const;
    };

    struct weapon_definition : public item_definition {
        float base_damage = 1.0f;
        int max_durability = 100;
        std::vector<classes::identifier> compatible_enchantments;
    };
}