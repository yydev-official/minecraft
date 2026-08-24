#include "identifier_manager.hpp"

std::unordered_set<int> identifier_manager::used_ids;
int identifier_manager::next_id = 1;

bool identifier_manager::register_id(const identifier& id) {
    if (is_used(id)) {
        return false; // Already used
    }

    used_ids.insert(id.id);
    return true;
}

identifier identifier_manager::generate_unique_id() {
    while (is_used(identifier(next_id))) {
        next_id++;
    }
    identifier new_id(next_id++);
    used_ids.insert(new_id.id);
    return new_id;
}

void identifier_manager::release_id(const identifier& id) {
    used_ids.erase(id.id);
}