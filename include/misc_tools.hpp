#pragma once
#include <functional>
#include <fmt/format.h>

namespace d_common {
class MiscTools {
public:
    /**
     * Get or throw.
     * \tparam T type to get. Can be reference (for example string&) or value.
     * \param _file_diag_ Use D_U_M_FILE_LINE_TRACE_IN_FMT for this and `_line_diag_` parameter.
     * \param _line_diag_ Use D_U_M_FILE_LINE_TRACE_IN_FMT for this and `_file_diag_` parameter.
     * \param getter Function that will get.
     * \param invalid_val Value that is considered invalid if returned by `getter`.
     * \return Value returned by getter.
     */
    template <typename T, typename exception_type>
    static T get_or_throw(const char* _file_diag_, const char* _line_diag_,
                   std::function<T()> getter, T invalid_val = {}) {
        T tv = getter();
        if (tv == invalid_val) {
            throw exception_type(
                fmt::format("Bad data at {}{}. Function returned `{}`, which is "
                            "considered invalid.",
                            _file_diag_, _line_diag_, invalid_val));
        }
        return tv;
    }
};
}  // namespace d_common