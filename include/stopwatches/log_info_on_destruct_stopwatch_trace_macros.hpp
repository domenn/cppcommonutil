#pragma once
#include "log_info_on_destruct_stopwatch.hpp"
#include "../convenience_diagnostics.hpp"
#include <plog/Log.h>
#include <fmt/format.h>

#define _INTERNAL__IMPL___D_CREATE_THE_NAME1(X, Y) X##Y
#define _INTERNAL__IMPL___D_CREATE_THE_NAME(string, s) \
    _INTERNAL__IMPL___D_CREATE_THE_NAME1(string, s)

#define _INTERNAL__IMPL___D_DIAG_INFO_TIMING(string, item) item _INTERNAL__IMPL___D_CREATE_THE_NAME(_secret_stopwatch_l_, __LINE__) ( fmt::format("SW: [{}@{}{}] " string, plog::util::processFuncName(PLOG_GET_FUNC()), __LINE__, PLOG_GET_FILE()

#define DIAG_INFO_TIMING(string) _INTERNAL__IMPL___D_DIAG_INFO_TIMING(string, d_common::LogInfoOnDestructStopwatch) ))
#define DIAG_INFO_TIMING_MULTI_ITEM(string, ...) _INTERNAL__IMPL___D_DIAG_INFO_TIMING(string, d_common::LogInfoOnDestructStopwatch), __VA_ARGS__ ))

#define DIAG_INFO_TIMING_DETAILED(string) _INTERNAL__IMPL___D_DIAG_INFO_TIMING(string, d_common::ConfigurableLogOnDestructStopwatch) ))
#define DIAG_INFO_TIMING_MULTI_ITEM_DETAILED(string, ...) _INTERNAL__IMPL___D_DIAG_INFO_TIMING(string, d_common::ConfigurableLogOnDestructStopwatch), __VA_ARGS__ ))