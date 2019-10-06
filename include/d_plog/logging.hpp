#pragma once

#include <plog/Log.h>
#include <plog/Appenders/ColorConsoleAppender.h>
#include "loggers.hpp"
#include "encoding.hpp"

namespace app_logging {
#ifdef IS_THIS_UNIT_TEST

namespace internal {
template <typename str_type>
inline void push_to_stringstream(std::ostringstream &ss, const str_type &word) {
    std::cerr << "NOT SUPPORTED TEMPLATE SPECIALIZATION !!!!\n  " << __FILE__ << __LINE__
              << std::endl;
    exit(2);
}

template <>
inline void push_to_stringstream(std::ostringstream &ss, const std::string &word) {
    ss << word << std::endl;
}

template <>
inline void push_to_stringstream(std::ostringstream &ss, const std::wstring &word) {
    ss << d_common::utf16_to_utf8(word);
}
}  // namespace internal

class MusicTestsAppender : public plog::IAppender {
    static MusicTestsAppender testTextAppender;
    inline static std::vector<plog::util::nstring> appender_msgs;

    template <typename T>
    class EnsureVectorErasure {
        std::vector<T> &vec_;

    public:
        EnsureVectorErasure() = delete;

        EnsureVectorErasure(std::vector<T> &vec) : vec_(vec) {}

        ~EnsureVectorErasure() { vec_.clear(); }
    };

public:
    void write(const plog::Record &record)
        override  // This is a method from IAppender that MUST be implemented.
    {
        plog::util::nstring str = plog::TxtFormatter::format(
            record);  // Use the formatter to get a string from a record.

        appender_msgs.push_back(str);  // Store a log message in a list.
    }

    static bool empty() { return appender_msgs.empty(); }

    static std::string get_logged_msgs() {
        std::ostringstream result;
        for (const auto &message : appender_msgs) {
            app_logging::internal::push_to_stringstream(result, message);
        }
        return result.str();
    }

    static std::string get_logged_msgs_and_reset() {
        EnsureVectorErasure fun(appender_msgs);
        return get_logged_msgs();
    }

    static MusicTestsAppender &get_static_inst() { return testTextAppender; }
};

#endif  // IS_THIS_UNIT_TEST

template <int log_number>
inline void add_appeners(const plog::Severity lvl, plog::IAppender *app1,
                         plog::IAppender *app2) {
    plog::init<log_number>(lvl, app1)
        .addAppender(app2)
#ifdef IS_THIS_UNIT_TEST
        .addAppender(&MusicTestsAppender::get_static_inst())
#endif
        ;
}

/**
 * Default logger setup with console and file. If already init-ed, does nothing.
 * Needs full filename. To construct it, use for example function to get exe dir
 * @param full_filename If only filename is passed, will be created in working directory
 * (not exe directory). If full (absolute) path is passed, it will be used.
 * @param level logging level
 */
inline void setup_logger(const std::string &full_filename = "log.txt",
                         const plog::Severity level = plog::info,
                         const plog::Severity timers_diagnostics_level = plog::debug,
                         const plog::Severity low_priority_log_level = plog::warning) {
    if (plog::get()) {
        return;
    }
    static plog::RollingFileAppender<plog::TxtFormatter> file_appender(
        full_filename.c_str(), 320000,
        4);  // Create the 1st appender.
    static plog::ColorConsoleAppender<plog::TxtFormatter>
        console_appender;  // Create the 2nd appender.

    add_appeners<PLOG_DEFAULT_INSTANCE>(level, &file_appender, &console_appender);
    add_appeners<ALWAYS_INFO_LOGGER>(plog::info, &file_appender, &console_appender);

    if (timers_diagnostics_level > plog::none) {
        add_appeners<TIMING_DIAGNOSTICS_LOGGER>(timers_diagnostics_level, &file_appender,
                                                &console_appender);
    }
    if (low_priority_log_level > plog::none) {
        add_appeners<LOW_PRIORITY_LOGGER>(low_priority_log_level, &file_appender,
                                          &console_appender);
    }
}

// TODO prio 0 .. find first suitable newline. And split on it (no random lications in
// between).
// TODO prio 0 .. think about bringing different file for "always-info" logger ...

template <int log_num>
void split_and_log(const std::string &full_msg, plog::Severity log_level) {
    constexpr auto LINES_ONE_MSG = 65536;
    size_t where_i_am = 0;
    while (true) {
        const auto position_before = where_i_am;
        where_i_am += LINES_ONE_MSG;
        where_i_am = (std::min)(where_i_am, full_msg.size());
        while (full_msg[where_i_am] != '\n' && full_msg[where_i_am] != '\0') {
            ++where_i_am;
        }
        if (where_i_am < full_msg.size()) {
            std::string_view view(full_msg.data() + position_before,
                                  where_i_am - position_before);
            PLOG_(log_num, log_level) << view;
            // Throw a newline ('\n') out of the log item.
            ++ where_i_am;
        } else {
            PLOG_(log_num, log_level) << full_msg.data() + position_before;
            break;
        }
    }
}

}  // namespace app_logging
