#pragma once
#include <unordered_set>
#include "game/classes/identifier/identifier.hpp"

namespace mcxx {
    namespace classes {
        class identifier_manager {
            public:
                static bool is_used(const identifier& id);
                static bool register_id(const identifier& id);
                static identifier generate_unique_id();
                static void release_id(const identifier& id);
        };

    } // namespace classes
} // namespace mcxx