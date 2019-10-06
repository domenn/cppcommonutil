#ifndef _WIN32
#pragma once

#include <string>

namespace process_runner {
class Linux {
public:
    static void start_non_executable_file(const std::string& path);
    static std::string run_exe_and_get_stdout(const std::string& path);
};

}
#endif
