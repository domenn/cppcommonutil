

add_library(dCppUtilsLib::dCppUtilsLib IMPORTED)
set_property(TARGET dCppUtilsLib PROPERTY IMPORTED_LOCATION ".")

message(STATUS "including .. interface and stuff.")

target_include_directories(dCppUtilsLib INTERFACE
    $<BUILD_INTERFACE:C:/Users/Domen/CLionProjects/CppHelpersBase/d_cpp_util/include>
    $<INSTALL_INTERFACE:C:/Users/Domen/CLionProjects/CppHelpersBase/d_cpp_util/include>  # <prefix>/include/mylib
    )