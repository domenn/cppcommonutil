#include <d_spdlog/spd_logging.hpp>
#include <memory>
#include <vector>

#include <spdlog/sinks/msvc_sink.h>
#include <map>
#include <str_util.hpp>

#include "colorful_sink_factory_singleton.hpp"

spdlog::filename_t log_filename{};
bool log_to_file{true};
spdlog::level::level_enum my_level{spdlog::level::level_enum::info};

std::map<std::string, std::shared_ptr<spdlog::logger>> all_loggers;

std::shared_ptr<spdlog::sinks::basic_file_sink_mt> get_file_sink() {
  // This forces the file_sink_ variable to always be in shared libray (shared
  // between lib and consumer app).

  static std::shared_ptr<spdlog::sinks::basic_file_sink_mt> file_sink_ =
      std::make_shared<spdlog::sinks::basic_file_sink_mt>(log_filename.empty() ? "logs.log" : log_filename);

  return file_sink_;
}

void spdl::spdlog_setup_default() {
  constexpr const char DEFAULT_LOGGER_NAME[] = "Spdlogs";
  spdlog::set_default_logger(get_as_shared(DEFAULT_LOGGER_NAME));
  SPDLOG_DEBUG("Test logging as debug.");
}
std::shared_ptr<spdlog::logger> spdl::make_new_logger(const char *name) {
  std::vector<spdlog::sink_ptr> sinks;
  sinks.reserve(3);

  sinks.emplace_back(get_file_sink());
#ifdef _WIN32
#ifndef NDEBUG
  sinks.emplace_back(std::make_shared<spdlog::sinks::msvc_sink_mt>());
#endif
#endif
  sinks.emplace_back(internal::ColorfulSinkFactorySingleton::make_colorful_stdout());
  for (auto &sink : sinks) {
    sink->set_level(my_level);
  }
  sinks.shrink_to_fit();
  auto shartedptr_lg = std::make_shared<spdlog::logger>(name, sinks.begin(), sinks.end());
  shartedptr_lg->set_level(my_level);

  shartedptr_lg->set_pattern("%^%d.%m.%Y %H:%M:%S.%e %n [%l, %s:%#] - %v%$");

  return shartedptr_lg;
}
std::shared_ptr<spdlog::logger> &spdl::get_as_shared(const char *name) {
  if (auto iterator = all_loggers.find(name); iterator != all_loggers.end()) {
    return iterator->second;
  }
  auto created = all_loggers.emplace(name, make_new_logger(name));
  return created.first->second;
}
spdlog::logger *spdl::get(const char *name) { return get_as_shared(name).get(); }
void spdl::set_log_file(spdlog::filename_t file_name) { log_filename = std::move(file_name); }
void spdl::spdlog_setup(const spdl::SpdlogConfig &cfg) {
  if (!cfg.file_name.empty()) {
    set_log_file(cfg.file_name);
  }

  my_level = cfg.level;
  log_to_file = cfg.log_to_file;

  if (!cfg.default_logger_name.empty()) {
    spdlog::set_default_logger(get_as_shared(cfg.default_logger_name.c_str()));
  }
}
spdlog::level::level_enum spdl::level_from_str(std::string level_str) {
  if (level_str.empty()) {
    return my_level;
  }
  if (level_str.size() == 1) {
  	std::for_each(level_str.begin(), level_str.end(), [](auto& c) {c = ::toupper(c); });
    auto idx =
        std::find(std::begin(spdlog::level::short_level_names), std::end(spdlog::level::short_level_names), level_str) -
        std::begin(spdlog::level::short_level_names);
    return static_cast<spdlog::level::level_enum>(idx);
  }
  std::for_each(level_str.begin(), level_str.end(), [](auto& c) {c = ::tolower(c); });
  return spdlog::level::from_str(level_str);
}
void spdl::set_level(const char *name, spdlog::level::level_enum new_lvl) {
  spdl::get(name)->set_level(new_lvl);
  std::for_each(spdl::get(name)->sinks().begin(), spdl::get(name)->sinks().end(), [new_lvl](auto &itm) {
    itm->set_level(new_lvl);
  });
}

spdl::SpdlogCfgBuilder spdl::SpdlogConfig::build() { return {}; }