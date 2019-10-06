#pragma once

#if defined(_WIN32) && defined(_MSC_VER)

#include "encoding.h"
#define WIDE_IF_MS_COMPILER(x) d_common::utf8_to_utf16(x)

#else
#define WIDE_IF_MS_COMPILER(x) x
#endif