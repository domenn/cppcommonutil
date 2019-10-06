
#include <application_files.hpp>
#include <filesystem>
#include <sago/platform_folders.h>

namespace app_files_internal {
//std::string combine_paths_and_create_dirs() {
//
//}
//std::filesystem::path
}

std::string d_common::application_files::get_app_folder(bool create_parents) {
  // auto dir =
  return {};
}
std::string d_common::application_files::file_in_app_folder(const std::string &app_name,
                                                            const std::string &file_name,
                                                            bool create_parents) {
  auto dir = std::filesystem::path(sago::getDataHome()) / app_name;
  if (create_parents && !std::filesystem::exists(dir)) {
    std::filesystem::create_directories(dir);
  }
  dir /= file_name;
  return dir.make_preferred().u8string();
}
