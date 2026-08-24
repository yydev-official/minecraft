#include "registry.hpp"

template<typename T>
T item_definition::get_attribute(const std::string& key, T defaultValue) const {
    auto it = attributes.find(key);
    if (it != attributes.end() && std::holds_alternative<T>(it->second)) {
        return std::get<T>(it->second);
    }
    return defaultValue;
}