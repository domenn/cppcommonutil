# Domen's common C++ code

Some misc utilities that I use in  my hobby C++ projects.

## What's inside

String utilities, filename utilities (some things are redundant due to addition of c++17 std::filesystem). Some helper
code for logging (SPDLOG library). Error formatting code.

A lot of experimental code.

## Usage

Currently tested and supported way is to add this repository as cmake subdirectory and link target named dCppUtilsLib.
This will also add required flags and include directories to your target.
Cmake build and install has not been tested. 

##### Example:

```cmake
add_executable(CppHelpersExample main.cc)
target_link_libraries(CppHelpersExample PRIVATE dCppUtilsLib)
```

## State of this library, contributing

Many things don't work yet or do not work in most scenarios. This should compile at least on Windows (GCC, VS) and
Linux (GCC). Currently this can be used as a static library. Dynamic library mode may work, but has not been tested.

There are some dependencies. Cmake utils are added to download them using `ExternalProject_add`. Some libraries or parts
of code need to be enabled with CMake variables (the whole library may not compile without them).

If you find any part of this utility useful, feel free to submit pull request adding the functionality you need or
supporting currently unsupported scenarios or improving my code style or design. **Feel free to open an issue with any 
problem or missing feature that you encounter. Or any question that you have whatsoever.** I only implement and test things that I need, but i will gladly add
the requested features, provided they are within my capabilities.
