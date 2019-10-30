#pragma once

#ifdef D_USING_SPDLOG

#include <d_spdlog/spd_logging.hpp>

// Map level names ...
#ifdef TRACE
#define VERBOSE TRACE
#endif
#define WARNING WARN
#define WARNING WARN

#define D_ANY_LOG_DEFAULT_IMPL(level, ...) SPDLOG_##level(__VA_ARGS__)

#define D_LOG_DEFAULT(level, ...) D_ANY_LOG_DEFAULT_IMPL(level, __VA_ARGS__)
#define D_LOG(level, namestr, ...) SPDLOG_LOGGER_##level(spdl::get(namestr), __VA_ARGS__)

#define D_LOGT(namestr, ...) SPDLOG_LOGGER_TRACE(spdl::get(namestr), __VA_ARGS__)
#define D_LOGD(namestr, ...) SPDLOG_LOGGER_DEBUG(spdl::get(namestr), __VA_ARGS__)
#define D_LOGI(namestr, ...) SPDLOG_LOGGER_INFO(spdl::get(namestr), __VA_ARGS__)
#define D_LOGW(namestr, ...) SPDLOG_LOGGER_WARN(spdl::get(namestr), __VA_ARGS__)
#define D_LOGE(namestr, ...) SPDLOG_LOGGER_ERROR(spdl::get(namestr), __VA_ARGS__)
#define D_LOGF(namestr, ...) SPDLOG_LOGGER_CRITICAL(spdl::get(namestr), __VA_ARGS__)

#define D_DLOGT(...) SPDLOG_TRACE(__VA_ARGS__)
#define D_DLOGD(...) SPDLOG_DEBUG(__VA_ARGS__)
#define D_DLOGI(...) SPDLOG_INFO(__VA_ARGS__)
#define D_DLOGW(...) SPDLOG_WARN(__VA_ARGS__)
#define D_DLOGE(...) SPDLOG_ERROR(__VA_ARGS__)
#define D_DLOGF(...) SPDLOG_CRITICAL(__VA_ARGS__)

#elif defined(D_USING_PLOG)
// NOTE: Untested. Probably doesn't work.
#define D_LOG_DEFAULT(level, ...) PLOG_##level << fmt::format(__VA_ARGS__);
#define D_LOG(level, namestr, ...) PLOG_##level << namestr ": " << fmt::format(__VA_ARGS__);
#else
#define D_LOG(...)
#define D_LOG_DEFAULT(...)
#endif