

#include <d_exceptions.hpp>
#include <utility>
#include <fmt/format.h>

d_common::exc::TracingException::TracingException(std::exception_ptr exc, const char* const funct, const char* const fn,
                                                  int ln)
    : wrapped_exception_(std::move(exc)) {
  trace_str_.reserve(256);
  trace(funct, fn, ln);
}
void d_common::exc::TracingException::trace(const char* const funct, const char* const fn, int ln) {
  fmt::format_to(std::back_inserter(trace_str_), "\n ({}): {}, {}:{}", trace_cnt_++, funct, fn, ln);
}
const char* d_common::exc::TracingException::what() const noexcept  {
  what_item_.resize(0);
  try {
    if (wrapped_exception_) {
      std::rethrow_exception(wrapped_exception_);
    }
  } catch (const std::exception& e) {
    what_item_.append("  ").append(e.what()).append("; Of type: ").append(typeid(e).name());
  }
  what_item_ += trace_str_;
  return what_item_.c_str();
}
