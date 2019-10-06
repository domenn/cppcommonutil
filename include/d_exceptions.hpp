#pragma once
#include <string>
#include <stdexcept>
#include <utility>
#include <spdlog/spdlog.h>

class DOriginalExceptionHolder {
public:
  std::exception_ptr wrapped_exception_;

  DOriginalExceptionHolder(std::exception_ptr wrappedException) : wrapped_exception_(std::move(wrappedException)) {}
};

namespace d_common::exc {

template <typename EX_TPE, typename... Args>
EX_TPE create_with_file_line(const std::string &fn, int ln, const std::string &x_txt, Args &&... arg) {
  //  EX_TPE some_x(x_txtstd::forward<Args>(arg)...);
  //  some_x.
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "InfiniteRecursion"
inline std::string write_exception(const std::exception &e, int level = 1, std::string &&accumulated = {}) {
  accumulated.append("\n ").append(std::to_string(level)).append(": ").append(e.what());
  try {
    std::rethrow_if_nested(e);
  } catch (const std::exception &e) {
    auto original_exception = dynamic_cast<const DOriginalExceptionHolder*>(&e);
    if(original_exception){
      SPDLOG_WARN("Cast OK !!!");
    }else{
      SPDLOG_ERROR("!! Cast NOT OK !  !!!");
    }
    return write_exception(e, level + 1, std::move(accumulated));
  } catch (...) {
  }
  return accumulated;
}
#pragma clang diagnostic pop

#define TRACING_X_TRACE SPDLOG_FUNCTION, SPDLOG_FILE_BASENAME(__FILE__), __LINE__

class TracingException : public std::exception {
  std::string trace_str_;
  mutable std::string what_item_;
  // std::unique_ptr<std::exception> wrapped_exc_;
  // const std::exception & wrapped_exc_;
  std::exception_ptr wrapped_exception_;
  int trace_cnt_{1};

public:
  const char *what() const noexcept override;
  void trace(const char *funct, const char *fn, int ln);
  explicit TracingException(std::exception_ptr exc, const char *funct, const char *fn, int ln);
};

class DumbException {};

class DumbTracingException : public TracingException, DumbException {
public:
  explicit DumbTracingException(std::exception_ptr exc, const char *funct, const char *fn, int ln)
      : TracingException(std::move(exc), funct, fn, ln) {}
};

template <typename WrappedXType>
class StaticPolimorphicException : std::exception {
public:
  WrappedXType wrapped_exception_;

  explicit StaticPolimorphicException(WrappedXType t) : wrapped_exception_(std::move(t)) {}

private:
  const char *what() const noexcept override { return wrapped_exception_.what(); }
};

class DAnyException : public std::runtime_error {
public:
  explicit DAnyException(const std::string &__arg) : runtime_error(__arg) {}
  explicit DAnyException(const char *string) : runtime_error(string) {}
};

class ProcessRunnerException : public DAnyException {
public:
  explicit ProcessRunnerException(const std::string &__arg) : DAnyException(__arg) {}
  explicit ProcessRunnerException(const char *string) : DAnyException(string) {}
};

class OsApiException : public DAnyException {
public:
  explicit OsApiException(const std::string &__arg) : DAnyException(__arg) {}
  explicit OsApiException(const char *string) : DAnyException(string) {}
};

}