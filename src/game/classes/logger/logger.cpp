#include "logger.h"

#include <cstdio>
#include <string>
#include <vector>
#include <mutex>
#include <WinUser.h>
#include <ctime>

std::vector<std::string> cc::classes::logger::s_logs;
std::mutex cc::classes::logger::s_log_mutex;

void cc::classes::logger::log(const std::string& msg, ...)
{
    std::lock_guard<std::mutex> lock(s_log_mutex);
    s_logs.push_back(msg);

    time_t timestamp = time(&timestamp);
    struct tm datetime = *localtime(&timestamp);

	printf("[CC] - [LOG] [%d]: %s\n", datetime.tm_hour, msg.c_str());
}

void cc::classes::logger::fatal_error(const char* message, ...) {
    MessageBoxA(nullptr, message, "Fatal Error", MB_ICONERROR | MB_OK);
    printf("[LOG] %s\n", message);
}

const std::vector<std::string>& cc::classes::logger::get_logs()
{
    std::lock_guard<std::mutex> lock(s_log_mutex);
    return s_logs;
}
