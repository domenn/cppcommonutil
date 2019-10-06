
#include <fmt/format.h>
#include <d_log_wrapper.hpp>

#include <stopwatches/log_info_on_destruct_stopwatch.hpp>

d_common::ConfigurableLogOnDestructStopwatch::~ConfigurableLogOnDestructStopwatch() {
  D_LOG_DEFAULT(INFO, fmt::format(log_msg_, as_string()));
  // spdl::spdlog_setup_default()
}

d_common::LogInfoOnDestructStopwatch::~LogInfoOnDestructStopwatch() {
  do_logging(this);
}
std::string d_common::LogInfoOnDestructStopwatch::make_msg() const { return fmt::format(log_msg_, expired_ms()); }
d_common::SpdlogNamedOnDestructSw::~SpdlogNamedOnDestructSw() { do_logging(this); }

spdlog::level::level_enum d_common::SpdlogNamedOnDestructSw::level() const { return level_; }
spdlog::logger* d_common::SpdlogNamedOnDestructSw::logger() const { return spdl::get(logger_name_.c_str()); }
spdlog::level::level_enum d_common::LogInfoOnDestructStopwatch::level() const {
  return spdlog::level::level_enum::info;
}
spdlog::logger* d_common::LogInfoOnDestructStopwatch::logger() const { return spdlog::default_logger_raw(); }
void d_common::LogInfoOnDestructStopwatch::do_logging(const LogInfoOnDestructStopwatch* logger) {
  if (!logger->destructed_) {
    SPDLOG_LOGGER_CALL(logger->logger(), logger->level(), logger->make_msg());
    logger->destructed_ = true;
  }
}
