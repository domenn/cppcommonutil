#pragma once
#include <string>

namespace d_common::windows {
/**
 * \brief Get some error info from WinApi. A little cryptic function .. Find usage example
 * in windows process runner.
 *
 * \param error_code returned by GetLastError().
 * \param str_lpszFunction user defined string.
 * \return function (human friendly name), lp_msg_buf, lp_display_buf
 */
std::tuple<std::string, std::string, std::string> format_windows_error_legacy(
    uint32_t error_code, const std::string& str_lpszFunction);

std::string format_windows_error(
    uint32_t error_code, const std::string& str_lpszFunction);
}  // namespace d_common::windows
