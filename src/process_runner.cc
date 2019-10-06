#include "process_runner.hpp"
#include <string_view>

#ifdef _WIN32
#include <windows.h>
#include <encoding.hpp>
#include <win/windows_process_runner.hpp>
#else
#include <linux/linux.hpp>
#endif


using namespace std;

#ifdef _WIN32

#endif
//
std::string d_common::run_executable(std::string&& program_to_run,
                                     bool show_console, bool redirect_std,
                                     bool wait_until_exec, bool shell) {
#ifdef _WIN32
	windows::WindowsProcessRunner runner(std::move(program_to_run));
    if (redirect_std) {
        runner.redirect_std_out(true);
    }
    runner.run(show_console, wait_until_exec);
    return runner.produced_stdout_steal();
#else
    return process_runner::Linux::run_exe_and_get_stdout(program_to_run);
#endif
}

void d_common::start_non_executable_file(std::string&& path)
{
#ifdef _WIN32
    std::wstring filename_win_frindly = d_common::utf8_to_utf16(path);
    ShellExecuteW(0, 0, filename_win_frindly.c_str(), 0, 0, SW_SHOW);
#else
    process_runner::Linux::start_non_executable_file(path);
#endif
}