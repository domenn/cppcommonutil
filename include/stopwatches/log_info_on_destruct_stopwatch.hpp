#pragma once
#include <string>
#include "detailing_output_stopwatch.hpp"
// TODO d : Fix fmt template wall.
// #include <fmt/ostream.h>
namespace d_common {

/**
 * Uses plog and fmt to write a message on destructor call. Message to be written is
 * provied in object's constructor. Uses fmt lib to format the message. Construct with the
 * message that the destructor shall print. Message must conta
 */
class LogInfoOnDestructStopwatch : public Stopwatch {
  const std::string log_msg_;

protected:
  // Destructor doesn't call virtual methods in subclasses .. we need a workaround.
  mutable bool destructed_{false};

  virtual std::string make_msg() const;
  virtual spdlog::logger* logger() const;
  virtual spdlog::level::level_enum level() const;

  static void do_logging(const LogInfoOnDestructStopwatch* logger);

public:
  /**
   * Start a stopwatch, store the message to write at destruct time.
   *
   * @param log_msg Message that MUST be compatibile with fmt: must contain exactly one
   * {}. fmt::format will insert number into that placeholder.
   */
  explicit LogInfoOnDestructStopwatch(std::string log_msg) : log_msg_(std::move(log_msg)) {}

  ~LogInfoOnDestructStopwatch() override;
};

class SpdlogNamedOnDestructSw : public LogInfoOnDestructStopwatch {
  const std::string logger_name_;
  spdlog::level::level_enum  level_;

  spdlog::logger* logger() const override;
  spdlog::level::level_enum level() const override;

public:
  SpdlogNamedOnDestructSw(std::string logger_name, std::string log_msg, spdlog::level::level_enum level = spdlog::level::info)
      : LogInfoOnDestructStopwatch(std::move(log_msg)), logger_name_(std::move(logger_name)), level_(level) {}
  ~SpdlogNamedOnDestructSw() override;
};

class ConfigurableLogOnDestructStopwatch : public DetailingOutputStopwatch {
  const std::string log_msg_;

public:
  /**
   * Similiar to `LogInfoOnDestructStopwatch`, the difference is, time is written in
   * microseconds. API, usage (methods) is exactly the same.
   *
   * @param log_msg As other class.
   *
   * @see class LogInfoOnDestructStopwatch
   */
  explicit ConfigurableLogOnDestructStopwatch(std::string log_msg) : log_msg_(std::move(log_msg)) {}
  ~ConfigurableLogOnDestructStopwatch() override;
};

}  // namespace d_common