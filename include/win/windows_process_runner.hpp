#pragma once
#ifdef _WIN32
#include <windows.h>
#include <string>
#include <fmt/format.h>

namespace d_common::windows {

class HandleWrapper {
    HANDLE ptr_{nullptr};
    bool armed_{false};

public:
    void arm(HANDLE ptr) {
        ptr_ = ptr;
        armed_ = true;
    }

    void arm() { armed_ = true; }

    void disarm() { armed_ = false; }
    void close_now() {
        armed_ = false;
        CloseHandle(ptr_);
    }
    explicit HandleWrapper(HANDLE ptr) : ptr_(ptr), armed_(true) {}
    HandleWrapper() = default;
    ~HandleWrapper() {
        if (armed_) {
            CloseHandle(ptr_);
        }
    }
    HANDLE* handle_ptr() { return &ptr_; }
    HANDLE handle() const { return ptr_; }
};

class WindowsProcessRunner {
    // TODO prio 9 seperate stderr handler and reader.
    typedef fmt::basic_memory_buffer<wchar_t, 702> win_wchar_buf_t;

    STARTUPINFOW startup_info_;
    PROCESS_INFORMATION process_information_;
    const std::string program_to_run_;
    DWORD new_process_flags_{CREATE_UNICODE_ENVIRONMENT};
    // Default value. Use max_wait_time function to change. Use INFINITE to infinite wait
    // or 0 for no wait.
    DWORD max_wait_time_{3600};
    bool redirect_std_out_{false};
    HandleWrapper h_stdout_read_{};
    HandleWrapper h_stdout_write_{};
    HandleWrapper h_stdin_write_{};
    HandleWrapper h_stdin_read_{};
    SECURITY_ATTRIBUTES security_attr_;
    std::string produced_stdout_;

    /**
     * \brief Used to for example get child's stdout. Returns handles if successful,
     * otherwise throws.
     * \return First handle is pipe for reading. Child uses it to send to us.
     * Second handle is for writing, so write to child.
     * Second pipe is set to no-inherit. So child will not get it!
     */
    void create_child_comms_pipe(HandleWrapper& reading_handle,
                                 HandleWrapper& writing_handle,
                                 const std::string& additional_err_info = {});

    win_wchar_buf_t path_to_char_buffer() const;

    void error_throw(const std::string& str_lpszFunction, const char * const func, int line) const;
    void read_stdout();

public:
    WindowsProcessRunner(std::string&& program_to_run);
    WindowsProcessRunner& redirect_std_out(bool swwitch);

    /**
     * \brief Runs the app according to received parameters and previously configured
     * state. If stdout/err are redirected, will store them into variables result_std*.
     * If redirecting std, the client app needs to be able to finish ... this class has
     * not been made very robust, to support long running apps or something like that.
     * If function finishes normally, it has been successful. In case of failure, it will
     * throw exception.
     *
     * \param show_console True to display seperate console for new process.
     * \param wait_to_close True to make this program wait the executed program. May be
     * required if we are reading std.
     */
    void run(bool show_console, bool wait_to_close);

    /**
     * \brief max_wait_time Set timeout for waiting on exec'd exe.
     * \param max_wait_time value to set in milliseconds.
     * \return reference to *this.
     */
    WindowsProcessRunner& max_wait_time(DWORD max_wait_time);
    ~WindowsProcessRunner();

	std::string && produced_stdout_steal();
};
}  // namespace d_common::windows
#endif