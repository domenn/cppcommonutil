
#pragma once

#include <string>

#ifdef _WIN32

#include <windows.h>

#endif
namespace d_common {
#ifdef _WIN32

inline std::string utf16_to_utf8(const std::wstring &utf16) {
    // Special case of empty input string
    if (utf16.empty()) {
        // Return empty string
        return {};
    }

    // "Code page" value used with WideCharToMultiByte() for UTF-8 conversion
    const UINT codePageUtf8 = CP_UTF8;

    // Safely fails if an invalid UTF-16 character is encountered
    const DWORD flags = WC_ERR_INVALID_CHARS;

    // Get the length, in chars, of the resulting UTF-8 string
    const int utf8Length = ::WideCharToMultiByte(codePageUtf8,                   // convert to UTF-8
                                                 flags,                          // conversion flags
                                                 utf16.c_str(),                  // source UTF-16 string
                                                 static_cast<int>(utf16.size()), // length of source UTF-16 string, in WCHARs
                                                 nullptr,                        // unused - no conversion required in this step
                                                 0,                              // request size of destination buffer, in chars
                                                 nullptr, nullptr) +
                           1; // unused
    if (utf8Length == 1) {
        // Conversion error
        return {};
    }

    // Allocate destination buffer to store the resulting UTF-8 string
    char *bytes = new char[utf8Length]{0};

    // Do the conversion from UTF-16 to UTF-8
    int result = ::WideCharToMultiByte(codePageUtf8,                   // convert to UTF-8
                                       flags,                          // conversion flags
                                       utf16.c_str(),                  // source UTF-16 string
                                       static_cast<int>(utf16.size()), // length of source UTF-16 string, in WCHARs
                                       bytes,                          // pointer to destination buffer
                                       utf8Length,                     // size of destination buffer, in chars
                                       nullptr, nullptr);              // unused
    if (result == 0) {
        return {};
    }

    auto returning = std::string(bytes);
    delete[] bytes;
    // Return resulting UTF-8 string
    return returning;
}

inline std::wstring utf8_to_utf16(const std::string &utf8) {
    int wchars_num = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, NULL, 0);
    auto *wstr = new wchar_t[wchars_num];
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), -1, wstr, wchars_num);
    std::wstring result(wstr);
    delete[] wstr;
    return result;
}

//#define UTF_STRING std::wstring
//#define WL(letters) L##letters
//#define UTF_CHAR_FROM_CHAR(charstar) utf8_to_utf16(charstar).c_str()
//#define UTF_STR_FROM_CHAR(charstar) utf8_to_utf16(charstar)

#else

inline std::wstring utf8_to_utf16(const std::string &utf8) {
    // NOT IMPLEMENTED
    return {};
}

inline std::string utf16_to_utf8(const std::wstring &utf16) {
    // NOT IMPLEMENTED
    return {};
}


//#define UTF_STRING std::string
//#define WL(letters) letters
//#define UTF_FROM_CHAR(charstar) charstar
//#define UTF_STR_FROM_CHAR(charstar) str::string(charstar)

#endif // _WIN32

} // namespace d_common

/*
#include "encoding.h"

#ifdef _WIN32

#include <windows.h>

#endif

#ifdef _WIN32

std::string d_common::utf16_to_utf8(const std::wstring &utf16)

std::wstring d_common::utf8_to_utf16(const std::string &utf8)

#else

std::wstring utf8_to_utf16(const std::string &utf8) {
    // NOT IMPLEMENTED
    return {};
}


std::string utf16_to_utf8(const std::wstring &utf16) {
    // NOT IMPLEMENTED
    return {};
}
#endif // _WIN32

 */