#pragma once
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

using attribute_value = std::variant<int, float, std::string, bool>;

struct item_definition {
    std::string id;
    std::string name;
    std::unordered_map<std::string, attribute_value> attributes;

    template<typename T>
    T get_attribute(const std::string& key, T defaultValue) const;
};

struct weapon_definition : public item_definition {
    float base_damage = 1.0f;
    int max_durability = 100;
    std::vector<std::string> compatible_enchantments;
};