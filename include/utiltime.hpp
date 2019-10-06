#pragma once

#include <chrono>
#include <iomanip>
#include <sstream>
#include <string>

#if defined(__unix__)
#define FUNC_LOCALTIME(time, structure) localtime_r(time, structure)
#else
#define FUNC_LOCALTIME(time, structure) localtime_s(structure, time)
#endif

namespace d_common {

constexpr char D_DOT_M_DOT_Y__HMS[] = "%d.%m.%y_%H.%M.%S";

template <typename chrono_thing>
inline std::ostringstream format_some_input(const char *format, chrono_thing now_time) {
    auto itt = std::chrono::system_clock::to_time_t(now_time);
    std::ostringstream ss;
    std::tm stdtm{};
    FUNC_LOCALTIME(&itt, &stdtm);
    ss << std::put_time(&stdtm, format);
    return ss;
}

inline std::ostringstream format_current_time_stringstream(const char *format) {
    return format_some_input(format, std::chrono::system_clock::now());
}

inline std::string format_current_time(const char *format) {
    return format_current_time_stringstream(format).str();
}

inline std::string current_time_h_m_s_ms() {
    auto now = std::chrono::system_clock::now();
    auto ss = format_some_input("%H:%M:%S", now);
    ss << '.' << std::setfill('0') << std::setw(3)
       << (std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) %
           1000)
              .count();
    return ss.str();
}

inline std::string currentISO8601TimeUTC() { return format_current_time("%FT%TZ"); }



}  // namespace d_common
#undef FUNC_LOCALTIME
