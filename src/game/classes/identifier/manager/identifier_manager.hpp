#pragma once
#include <unordered_set>
#include "identifier.hpp"

class identifier_manager {
    private:
        static std::unordered_set<int> used_ids;
        static int next_id;
        
    public:
        // Check if a specific identifier is already in use
        static bool is_used(const identifier& id) {
            return used_ids.find(id.id) != used_ids.end();
        }

        // Try to register an ID. Returns true if successful, false if it was already taken.
        static bool register_id(const identifier& id);

        // Automatically generate a brand new, unused identifier
        static identifier generate_unique_id();

        // Free up an ID when an entity is destroyed
        static void release_id(const identifier& id);
};