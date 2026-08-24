#pragma once
#include <string>

namespace mcxx::classes {
    struct identifier {
        int id = 0;
        
        identifier() = default;
        explicit identifier(int id) : id(id) {}

        static identifier parse(const std::string& str);

        int get_id() { return id; };

        // Comparison operators so identifiers can be used as keys in maps or compared
        bool operator==(const identifier& other) const {
            return id == other.id;
        }
        
        bool operator!=(const identifier& other) const {
            return id != other.id;
        }

        bool operator<(const identifier& other) const {
            return id < other.id;
        }
    };
};