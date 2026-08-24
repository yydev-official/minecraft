#pragma once
#include "manager/identifier_manager.hpp"
#include <string>

struct identifier : public identifier_manager {
    int id = 0;
    
    identifier() = default;
    explicit identifier(int id) : id(id) {}

    static identifier parse(const std::string& str);

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