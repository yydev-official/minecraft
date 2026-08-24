#pragma once

#include <vector>
#include <string>
#include <mutex>

namespace cc::classes {
    class logger {
    	public:
    		static void log(const std::string& msg, ...);
    		static void fatal_error(const char* message, ...);
    		static const std::vector<std::string>& get_logs();

    	private:
    		static std::vector<std::string> s_logs;
    		static std::mutex s_log_mutex;
	};
};