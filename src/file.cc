
#include "file.hpp"
#include <filesystem>
#include <fstream>

#ifdef _WIN32

#include "encoding.hpp"
#include <windows.h>

#else
#include <linux/limits.h>
#include <unistd.h>
#include <sys/stat.h>
#define MAX_PATH PATH_MAX
#endif

std::string d_common::file::getExecutableFullPath() {
#ifdef _WIN32
    wchar_t buffer[MAX_PATH];
    ::GetModuleFileNameW(nullptr, buffer, MAX_PATH);
    return d_common::utf16_to_utf8(buffer);
#else
    char buffer[MAX_PATH];
    readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    return std::string(buffer);
#endif
}

std::string d_common::file::getParentFolder(const std::string &path) {
    return path.substr(0, path.find_last_of("\\/"));
}

std::string d_common::file::extractFileNameWithoutPath(const std::string &path) {
    return path.substr(path.find_last_of("\\/") + 1);
}

std::string d_common::file::getExecutableName() {
    return extractFileNameWithoutPath(getExecutableFullPath());
}

std::string d_common::file::getCurrentWorkingDir() {
#ifdef _WIN32
    wchar_t cwd[MAX_PATH];
    if (_wgetcwd(cwd, MAX_PATH) != nullptr) {
        return d_common::utf16_to_utf8(cwd);
    } else {
        return {};
    }
#else
    char cwd[MAX_PATH];
    if (getcwd(cwd, sizeof(cwd)) != nullptr) {
        return std::string(cwd);
    } else {
        return std::string("");
    }
#endif
}

bool d_common::file::setCurrentWorkingDir(const std::string &workingDir) {
#if defined(_WIN32) && defined(_UNICODE)
    return _wchdir(d_common::utf8_to_utf16(workingDir).c_str()) == 0;
#else
    return chdir(workingDir.c_str()) == 0;
#endif
}

bool d_common::file::delete_file(const std::string &path) {
    if (std::filesystem::exists(path)) {
        return std::filesystem::remove(path);
    }
    return true;
}

d_common::file::FileExistMode d_common::file::file_exists(const std::string &path) {
#if defined(_WIN32)
    auto windows_path = d_common::utf8_to_utf16(path);
    const auto attr = GetFileAttributes(windows_path.c_str());
    if (INVALID_FILE_ATTRIBUTES == attr) {
        return FileExistMode::DOES_NOT_EXISTS;
    }
    if (attr & FILE_ATTRIBUTE_DIRECTORY) {
        return FileExistMode::EXISTS_AS_DIRECTORY;
    }
    return FileExistMode::EXISTS_AS_FILE;
#else
    struct stat buf {};
    if (stat(path.c_str(), &buf)) {
        // returned -1 .. means error
        return FileExistMode::DOES_NOT_EXISTS;
    }
    if (S_ISDIR(buf.st_mode)) {
        return FileExistMode::EXISTS_AS_DIRECTORY;
    }
    if (S_ISREG(buf.st_mode)) {
        return FileExistMode::EXISTS_AS_FILE;
    }
    return FileExistMode::DOES_NOT_EXISTS;
#endif
}

std::vector<char> d_common::file::read_all_bytes(char const *filename) {
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
    const std::ifstream::pos_type pos = ifs.tellg();

    std::vector<char> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(result.data(), pos);

    return result;
}

std::string d_common::file::read_all_text(char const *filename) {
    std::ifstream ifs(filename, std::ios::ate);
    const std::ifstream::pos_type pos = ifs.tellg();

    std::string result(pos, 0);

    ifs.seekg(0, std::ios::beg);
    ifs.read(result.data(), pos);

    return result;
}

void d_common::file::write_all_bytes(char const *filename,
                                     std::vector<char> const &bytes) {
    return write_all_bytes(filename, bytes.data(), bytes.size());
}

void d_common::file::write_all_bytes(char const *const filename, const char *const bytes,
                                     size_t const bytes_size) {
    std::ofstream(filename, std::ofstream::binary).write(bytes, bytes_size);
}

bool d_common::file::compare_files(const char *first, const char *second) {
    auto f1 = read_all_bytes(first);
    auto f2 = read_all_bytes(second);
    if (f1.size() != f2.size()) {
        return false;
    }
    return std::equal(f1.begin(), f1.end(), f2.begin());
}