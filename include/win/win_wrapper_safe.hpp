#pragma once

#include <cstring>
#include <d_log_wrapper.hpp>

namespace d_common::safe {

namespace pvt {
#if (__STDC_VERSION__ >= 201112L && defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ == 1)
inline size_t errsize(int errnum) { return strerrorlen_s(errnum); }
#else
inline constexpr size_t errsize(int) { return 238; }
#endif
}  // namespace pvt

inline std::string strerror(int errnum) {
#if _WIN32 || (__STDC_VERSION__ >= 201112L && defined(__STDC_LIB_EXT1__) && __STDC_WANT_LIB_EXT1__ == 1)
  const auto sz = pvt::errsize(errnum);
  std::string returning(pvt::errsize(errnum), '\0');
  strerror_s(returning.data(), sz, errnum);
  return returning;
#else
  return std::strerror(errnum);
#endif
}
}