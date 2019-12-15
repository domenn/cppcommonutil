#ifdef _WIN32

#include <encoding.hpp>
#include <win/windows_error_format.hpp>

#include <strsafe.h>
#include <windows.h>
#include <tuple>

class DeleteOnDestruct {
  LPVOID ptr_;

 public:
  explicit DeleteOnDestruct(LPVOID ptr) : ptr_(ptr) {}
  ~DeleteOnDestruct() { LocalFree(ptr_); }
};

std::tuple<std::string, std::string, std::string> d_common::windows::format_windows_error_legacy(
    uint32_t error_code, const std::string& str_lpszFunction) {
  LPVOID lpMsgBuf;
  std::wstring windows_friendly_parameter = utf8_to_utf16(str_lpszFunction);
  const wchar_t* lpszFunction = windows_friendly_parameter.c_str();

  FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                 NULL,
                 error_code,
                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                 (LPTSTR)&lpMsgBuf,
                 0,
                 NULL);

  const auto lpDisplayBuf = static_cast<LPVOID>(
      LocalAlloc(LMEM_ZEROINIT, (lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR)));
  StringCchPrintfW((LPTSTR)lpDisplayBuf,
                   LocalSize(lpDisplayBuf) / sizeof(TCHAR),
                   TEXT("%s failed with error %d: %s"),
                   lpszFunction,
                   error_code,
                   lpMsgBuf);
  DeleteOnDestruct deleter0(lpMsgBuf);
  DeleteOnDestruct deleter1(lpDisplayBuf);

  return std::make_tuple(d_common::utf16_to_utf8(lpszFunction),
                         d_common::utf16_to_utf8((LPTSTR)lpMsgBuf),
                         (d_common::utf16_to_utf8((LPTSTR)lpDisplayBuf)));
}
std::string d_common::windows::format_windows_error(uint32_t error_code, const std::string& str_lpszFunction) {
  return std::get<2>(format_windows_error_legacy(error_code, str_lpszFunction));
}

#endif  // _WIN32