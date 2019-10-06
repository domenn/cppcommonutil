#ifdef _WIN32
#include <win/windows_error_format.hpp>
#include <win/windows_process_runner.hpp>
#include <string>
#include <encoding.hpp>
#include <strsafe.h>
#include <d_exceptions.hpp>
#include <d_log_wrapper.hpp>
#include <convenience_diagnostics.hpp>

#undef ERROR

void d_common::windows::WindowsProcessRunner::create_child_comms_pipe(
    HandleWrapper& reading_handle, HandleWrapper& writing_handle,
    const std::string& additional_err_info) {
    if (!CreatePipe(reading_handle.handle_ptr(), writing_handle.handle_ptr(),
                    &security_attr_, 0)) {
      D_LOG(ERROR, "WinErr", "Throwing CreatePipe error.");
        error_throw("StdoutRd CreatePipe", __func__, __LINE__);
    }
    writing_handle.arm();
    reading_handle.arm();

    if (!SetHandleInformation(reading_handle.handle(), HANDLE_FLAG_INHERIT, 0)) {
      D_LOG(ERROR, "WinErr", "Throwing SetHandleInformation error.");
        error_throw("Stdout SetHandleInformation; " + additional_err_info, __func__,
                    __LINE__);
    }
}

d_common::windows::WindowsProcessRunner::win_wchar_buf_t
d_common::windows::WindowsProcessRunner::path_to_char_buffer() const {
    const auto wstring_program_name = utf8_to_utf16(program_to_run_);
    win_wchar_buf_t wchars_buf;
    wchars_buf.reserve(700);
    format_to(wchars_buf, L"{}{}", wstring_program_name, '\0');
    return wchars_buf;
}

d_common::windows::WindowsProcessRunner&
d_common::windows::WindowsProcessRunner::redirect_std_out(bool swwitch) {
    redirect_std_out_ = swwitch;
    return *this;
}

d_common::windows::WindowsProcessRunner::WindowsProcessRunner(
    std::string&& program_to_run)
    : program_to_run_(std::move(program_to_run)) {
    ZeroMemory(&startup_info_, sizeof(startup_info_));
    ZeroMemory(&process_information_, sizeof(process_information_));
    ZeroMemory(&security_attr_, sizeof(SECURITY_ATTRIBUTES));
}

void d_common::windows::WindowsProcessRunner::error_throw(
    const std::string& str_lpszFunction, const char* const func, int line) const {
    auto [friendly_name, lp_msg_buf, lp_display_buf] = format_windows_error_legacy(GetLastError(), str_lpszFunction);

    throw d_common::exc::ProcessRunnerException(fmt::format(
        "Windows API could not run the desired path: {}\nError was: "
        "{}\nMsgBuf: {}DisplayBuf: {}Called on: {}@{}",
        program_to_run_, friendly_name, lp_msg_buf, lp_display_buf, func, line));
}

void d_common::windows::WindowsProcessRunner::read_stdout()
#define BUFSIZE 128

// Read output from the child process's pipe for STDOUT
// and write to the parent process's pipe for STDOUT.
// Stop when there is no more data.
{
    DWORD dwRead, dwWritten, wtf;
    CHAR chBuf[BUFSIZE];
    BOOL bSuccess = FALSE;

    std::string read_data;

    auto smth =
        PeekNamedPipe(h_stdout_read_.handle(), chBuf, BUFSIZE, &dwRead, &dwWritten, &wtf);
    if (!smth) {
        auto err = GetLastError();
        if (err == ERROR_BROKEN_PIPE) {
          D_LOG(WARN, "Notifier",
                      "At {}{}{}{}\n  PeekNamedPipe returned ERROR_BROKEN_PIPE. Probably "
                      "there is no data in the started process."
                      "\n  Running: {}",
                      D_U_M_FUNC_FILE_LINE_TRACE_IN_FMT, program_to_run_);
        } else {
          D_LOG(ERROR, "WinErr", "Throwing PeekNamedPipe error.");
            error_throw("PeekNamedPipe", __func__, __LINE__);
        }
    }

    for (;;) {
        bSuccess = ReadFile(h_stdout_read_.handle(), chBuf, BUFSIZE, &dwRead, NULL);
        if (!bSuccess || dwRead == 0)
            break;
        read_data.insert(read_data.end(), chBuf, chBuf + dwRead);
    }
    if (read_data.empty()) {
        return;
    }
    produced_stdout_ = std::move(read_data);
}

void d_common::windows::WindowsProcessRunner::run(bool show_console, bool wait_to_close) {
    /*
Note1: Either peek ... or close the write handle before calling read. Otherwise reading
will block.

According to
https://stackoverflow.com/questions/33553837/win32-named-pipes-and-message-size-limits-is-the-old-64k-limit-still-applicabl:
Pipe buffers are huge. Gigabytes .. So I can simply call, wait, then close pipes and read.
     */

    /*
Yeah, that kind of work, but remember there is a bug in that example that prevents
ReadFromPipe from exiting. Before you call that function you need to call:
CloseHandle(g_hChildStd_OUT_Wr);. Also you'll probably want to wait for the child process
to exit before you close the handle to process with something like:
WaitForSingleObject(piProcInfo.hProcess, INFINITE);
     */

    if (show_console) {
        new_process_flags_ |= CREATE_NEW_CONSOLE;
    }

    if (redirect_std_out_) {
        security_attr_.nLength = sizeof(SECURITY_ATTRIBUTES);
        security_attr_.bInheritHandle = TRUE;
        security_attr_.lpSecurityDescriptor = NULL;

        create_child_comms_pipe(h_stdout_read_, h_stdout_write_, "stdout");

        startup_info_.dwFlags |= STARTF_USESTDHANDLES;

        // create_child_comms_pipe(h_stdin_read_, h_stdin_write_,
        //                        "stdin");
        startup_info_.hStdInput = h_stdin_read_.handle();

        auto hStdInput = GetStdHandle(STD_INPUT_HANDLE);

        startup_info_.hStdInput = hStdInput;
        startup_info_.hStdOutput = h_stdout_write_.handle();
        startup_info_.hStdError = h_stdout_write_.handle();
        // startup_info_.hStdError = h;
    }

    auto wchars_buf = path_to_char_buffer();
    BOOL bSuccess = CreateProcessW(
        NULL,               // We decide to pass full command and cmd as second parameter
        wchars_buf.data(),  // command line
        NULL,               // process security attributes
        NULL,               // primary thread security attributes
        TRUE,               // handles are inherited
                            // new_process_flags_,  // creation flags
        0,
        NULL,                    // use parent's environment
        NULL,                    // use parent's current directory
        &startup_info_,          // STARTUPINFO pointer
        &process_information_);  // receives PROCESS_INFORMATION

    if (!bSuccess) {
      D_LOG(ERROR, "WinErr", "Throwing CreateProcess error.");
        error_throw("CreateProcess", __func__, __LINE__);
    }
    // h_stdin_read_.close_now();
    // h_stdin_write_.close_now();
    HandleWrapper temp_deleter1(process_information_.hThread);
    HandleWrapper temp_deleter(process_information_.hProcess);
    if (wait_to_close) {
        WaitForSingleObject(process_information_.hProcess, max_wait_time_);
    }
    // h_stdout_read_.close_now();
    if (redirect_std_out_) {
        h_stdout_write_.close_now();
        D_LOG(TRACE, "WindowdProcessRunner", "Reading from redirected STDOUT.");
        read_stdout();
    }
}

d_common::windows::WindowsProcessRunner&
d_common::windows::WindowsProcessRunner::max_wait_time(const DWORD max_wait_time) {
    max_wait_time_ = max_wait_time;
    return *this;
}

std::string&& d_common::windows::WindowsProcessRunner::produced_stdout_steal() {
    return std::move(produced_stdout_);
}

d_common::windows::WindowsProcessRunner::~WindowsProcessRunner() {}
#endif