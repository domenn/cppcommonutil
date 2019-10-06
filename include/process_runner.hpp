#pragma once

#include <string>

namespace d_common {

std::string run_executable(std::string&& fileToRun, bool showConsoleWindow = false,
                           bool redirectStd = false, bool waitUntilExecuted = true,
                           bool shell = false);

/**
 * \brief Runs .txt or .png or anything. Opens it in the default associated app.
 * \param path Path to run.
 */
void start_non_executable_file(std::string&& path);
}  // namespace d_common