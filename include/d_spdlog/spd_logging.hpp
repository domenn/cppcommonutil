#pragma once

#include <spdlog/spdlog.h>
#include <memory>

namespace spdl {
class SpdlogCfgBuilder;

#define BUILDER_MACRO(variable)                    \
    template <typename A_uto>                      \
    SpdlogCfgBuilder &variable(const A_uto &val) { \
        building_obj.variable = val;               \
        return *this;                              \
    }
struct SpdlogConfig {
    friend class SpdlogBuilder;
    bool log_to_file{true};
    spdlog::filename_t file_name{};
    std::string default_logger_name{"Logger"};
    spdlog::level::level_enum level{spdlog::level::info};

	static SpdlogCfgBuilder build();
};

class SpdlogCfgBuilder {
    SpdlogConfig building_obj;

public:
    BUILDER_MACRO(log_to_file);
    BUILDER_MACRO(file_name);
    BUILDER_MACRO(default_logger_name);
    BUILDER_MACRO(level);

    operator SpdlogConfig && () { return std::move(building_obj); }
};

std::shared_ptr<spdlog::logger> make_new_logger(const char *name);
void spdlog_setup_default();
spdlog::logger *get(const char *name);
void set_level (const char *name, spdlog::level::level_enum new_lvl);
std::shared_ptr<spdlog::logger> &get_as_shared(const char *name);
void set_log_file(spdlog::filename_t file_name);
void spdlog_setup(const SpdlogConfig &cfg = {});
/**
 * Provide a string either full name or single letter name. Does not check name validity.
 * Passing empty string returns current level. If name is invalid, there is undefined
 * behaviour. Passing invalid string may unexpectedly return level off.
 *
 * @param level_str either one of SPDLOG_LEVEL_NAMES or short_level_names (in spdlog lib).
 * Case insensitive.
 * @return the level.
 */
spdlog::level::level_enum level_from_str(std::string level_str);
}  // namespace spdl
