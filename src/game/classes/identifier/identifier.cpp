#include "identifier.hpp"
#include "manager/identifier_manager.hpp"

identifier identifier::parse(const std::string& str) {
    return identifier_manager::register_id(identifier(std::stoi(str))) ? identifier(std::stoi(str)) : identifier(0);
}