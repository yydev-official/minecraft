#include "identifier.hpp"
#include "manager/identifier_manager.hpp"

namespace mcxx::classes {
    identifier identifier::parse(const std::string& str) {
        const int numeric_id = std::stoi(str);
        identifier temp_id(numeric_id);
        
        if (identifier_manager::register_id(temp_id)) {
            return temp_id;
        }
        
        return identifier(0);
    }
}