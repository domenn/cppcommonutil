#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace d_common::file {
enum FileExistMode { DOES_NOT_EXISTS = 0, EXISTS_AS_DIRECTORY = 1, EXISTS_AS_FILE = 2 };

std::string getCurrentWorkingDir();
std::string getExecutableName();
/**
 * Works cross platform, returned string is UTF-8.
 * @return Executable file, full path.
 */
std::string getExecutableFullPath();

std::string getParentFolder(const std::string &path);

std::string extractFileNameWithoutPath(const std::string &path);
bool setCurrentWorkingDir(const std::string &workingDir);

/**
 * \brief Delete file if it exists. Does nothing otherwise.
 *
 * \param path Path to file (full or relative) to delete.
 * \return True if either file does not exist or it does exist and was deleted
 * successfully. False otherwise.
 */
bool delete_file(const std::string &path);

/**
 * Cross platform (platform-native) check if file or directory exists. Returns what
 * exists. Can also be treated as boolean. example: if(fileExists(p)){//Path exists and is
 * folder or file } example: if(fileExists(p) == EXISTS_AS_DIRECTORY){//Path exists and is
 * folder }
 * @param path Path to check. On windows, both \ and / as path separator work.
 * @return DOES_NOT_EXISTS or EXISTS_AS_DIRECTORY or EXISTS_AS_FILE
 */
FileExistMode file_exists(const std::string &path);

std::vector<char> read_all_bytes(char const *filename);
std::string read_all_text(char const *filename);
void write_all_bytes(char const *filename, std::vector<char> const &bytes);
void write_all_bytes(char const *filename, const char *bytes, size_t bytes_size);
bool compare_files(const char *first,
                   const char *second);
}  // namespace d_common
