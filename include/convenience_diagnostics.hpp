#pragma once

constexpr char z_l_path_sep =
#ifdef _WIN32
    '\\';
#else
    '/';
#endif

using cstr = const char* const;

static constexpr cstr past_last_slash(cstr str, cstr last_slash) {
    return *str == '\0' ? last_slash
                        : *str == z_l_path_sep ? past_last_slash(str + 1, str + 1)
                                               : past_last_slash(str + 1, last_slash);
}

static constexpr cstr past_last_slash(cstr str) { return past_last_slash(str, str); }

#define ____D_U_M_STRINGIZE2(x) #x
#define ____D_U_M_STRINGIZE(x) ____D_U_M_STRINGIZE2(x)
#define D_U_M_LINE_STRING ____D_U_M_STRINGIZE(__LINE__)

#ifdef _MSC_VER
#define LOG_FUNC_NAME __FUNCTION__
#define _FUNC_ON_MS __FUNCTION__
#define _LINE_TRACE_ON_MS "[" __FUNCTION__ "@" L_LINE_STRING "] "
#else
#define LOG_FUNC_NAME __PRETTY_FUNCTION__
#define _FUNC_ON_MS
#define _LINE_TRACE_ON_MS
#endif

#define D_U_M_FILE_LINE_TRACE past_last_slash(__FILE__) + "@" D_U_M_LINE_STRING
#define D_U_M_STR_FILE_LINE_TRACE \
    std::string() + past_last_slash(__FILE__) + "@" D_U_M_LINE_STRING

#define D_U_M_STR_FUNC_FILE_LINE_TRACE \
    std::string("[") + LOG_FUNC_NAME + "]" + past_last_slash(__FILE__) + "@" D_U_M_LINE_STRING

#define D_U_M_FILE_LINE_TRACE_IN_FMT past_last_slash(__FILE__), "@" D_U_M_LINE_STRING

// pass {} four times! Like .. {}{}{}{}
#define D_U_M_FUNC_FILE_LINE_TRACE_IN_FMT \
    __func__, ", ", past_last_slash(__FILE__), ":" D_U_M_LINE_STRING
