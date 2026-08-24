#include "registry.hpp"

namespace mcxx::classes::items {
    template<typename T>
    T item_definition::get_attribute(const classes::identifier& key, T defaultValue) const {
        auto it = attributes.find(key);
        if (it != attributes.end() && std::holds_alternative<T>(it->second)) {
            return std::get<T>(it->second);
        }
        return defaultValue;
    }
}