#ifndef _WIN32

#include <linux/linux.hpp>
#include <win/win_wrapper_safe.hpp>
#include <d_exceptions.hpp>
#include <d_log_wrapper.hpp>
#include <contrib/errnoname.hpp>

#include <glib.h>
#include <gio/gio.h>
#include <convenience_diagnostics.hpp>
#include <filesystem>
#include <str_util.hpp>

class LinuxReturnCodeException : public d_common::exc::OsApiException {
 public:
  explicit LinuxReturnCodeException(int err_code, const std::string &__arg) : OsApiException(fmt::format(__arg,
                                                                                                         err_code)) {}
};

#define THROW_IF_ERROR_LINUX                                                             \
    if (!was_success) {                                                                  \
         assert(0 && "Untested here. finalizer unique ptr -- needs debugging and custom deleter.");          \
        if (error != nullptr) {                                                          \
                  std::unique_ptr<GError, decltype(&g_error_free)> finalizer(error, g_error_free);    \
            throw d_common::exc::OsApiException(fmt::format(                              \
                "Error: {}{}{}{}; More info: GQuark domain={};; gint code={};; "         \
                "gchar* message={}",                                                     \
                D_U_M_FUNC_FILE_LINE_TRACE_IN_FMT, error->domain, error->code,           \
                error->message));                                                        \
        } else {                                                                         \
            throw d_common::exc::ProcessRunnerException(                                          \
                fmt::format("Unknown error during linux app_exec operation!\n {}{}{}{}", \
                            D_U_M_FUNC_FILE_LINE_TRACE_IN_FMT));                         \
        }                                                                                \
    }

void process_runner::Linux::start_non_executable_file(const std::string &path) {
  gboolean was_success;
  GError *error = nullptr;

#if GLIB_CHECK_VERSION(2, 36, 0)
  D_DLOGT("Glib is quite new ... no need to g_type_init()");
#else
  g_type_init();
#endif

  was_success = g_app_info_launch_default_for_uri(
      ("file://" +
          std::filesystem::canonical(std::filesystem::path(path)).generic_string())
          .c_str(),
      NULL, &error);
  THROW_IF_ERROR_LINUX
}
std::string process_runner::Linux::run_exe_and_get_stdout(const std::string &path) {
  std::array<char, 128> buffer;
  std::string result;
  auto got_popen = popen(path.c_str(), "r");
  if (!got_popen) {
    throw d_common::exc::OsApiException(fmt::format(
        "Error: {}{}{}{}\n  Linux popen(). Error code {}({}) with message \"{}\".",
        D_U_M_FUNC_FILE_LINE_TRACE_IN_FMT,
        errno,
        errnoname(errno),
        d_common::safe::strerror(errno)));
  }
  std::unique_ptr<FILE, decltype(&pclose)> pipe(got_popen, pclose);
  if (!pipe) {
    throw d_common::exc::OsApiException(fmt::format(
        "Error: {}{}{}{}\n  Linux pipe(). Error code {}({}) with message \"{}\".",
        D_U_M_FUNC_FILE_LINE_TRACE_IN_FMT,
        errno,
        errnoname(errno),
        d_common::safe::strerror(errno)));
  }
  while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
    result += buffer.data();
  }
  // Manually pclose to get error code
  auto ret_code = pclose(pipe.release());
  if (ret_code) {
    auto out_ex_str = (result.empty() ? std::string{} : "\n  Program stdout: " + result);

    d_common::str_util::transform_replace_all(out_ex_str, '{', "<[");
    d_common::str_util::transform_replace_all(out_ex_str, '}', "]>");

    //  d_common::str_util::transform_replace_all(out_ex_str, '}', "{}}");
    // Ret std string may contain {} .. so use operator + not to mess with fmt.
    throw LinuxReturnCodeException(ret_code,
                                   fmt::format(
                                       "Linux process run returned non-zero error code: {{}}.\n  Running path {}.\n  At: {}{}{}{}",
                                       path,
                                       D_U_M_FUNC_FILE_LINE_TRACE_IN_FMT
                                   ) + out_ex_str

    );
  }
  return result;
}

#endif