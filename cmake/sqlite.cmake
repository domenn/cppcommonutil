#print_any_var(CMAKE_CURRENT_LIST_DIR)
#print_any_var(CMAKE_CURRENT_LIST_FILE)

if (DUTILCPP_ADD_SQLITE AND (NOT DUTILCPP_ADD_SQLITECPP))
  
  if (DUTILCPP_ADD_SQLITECPP)
    message(FATAL "Something is very wrong ...")
  endif ()
  
  option(SQLITE_ENABLE_THREADSAFE "build a thread-safe library, default = ON" ON)
  option(SQLITE_ENABLE_DYNAMIC_EXTENSIONS "support loadable extensions, default = ON" ON)
  option(SQLITE_ENABLE_FTS3 "version 3 of the full-text search engine, see also SQLITE_ENABLE_FTS4 which enables FTS3 as well, default = OFF" OFF)
  option(SQLITE_ENABLE_FTS3_PARENTHESIS "modifies the query pattern parser in FTS3 such that it supports operators AND and NOT and also allows query expressions to contain nested parenthesis, default = OFF" OFF)
  option(SQLITE_ENABLE_FTS4 "versions 3 and 4 of the full-text search engine, default = ON" ON)
  option(SQLITE_ENABLE_RTREE "include support for the R*Tree index extension" ON)
  option(SQLITE_ENABLE_COLUMN_METADATA "include APIs that provide access to meta-data about tables and queries, default = ON" ON)
  option(BUILD_SHARED_LIBS "" OFF)
  option(SQLITE_BUILD_LIB "Build libsqlite3, default = ON" ON)
  option(SQLITE_BUILD_SHELL "Build the sqlite3 shell, default = ON" ON)
  
  
  if (${CMK_SCRIPT_VERBOSE})
    include(${CMAKE_CURRENT_LIST_DIR}/util/get_target_props.cmake)
  endif ()
  set(DUTILCPPOPT_SQLITE3_VERSION "3290000" CACHE STRING "Sqlite version. Check https://sqlite.org/download.html to find available versions.")
  set(THIS_FILE_LIB_NAME SQLite3)
  set(PROJECT_CMK_EXT_PROJS_PRE_CUSTOM ${CMAKE_CURRENT_BINARY_DIR}/CmkExtProj)
  find_package(${THIS_FILE_LIB_NAME}
      HINTS
      ${PROJECT_CMK_EXT_PROJS_PRE_CUSTOM}
      NO_DEFAULT_PATH)
  
  if (NOT ${THIS_FILE_LIB_NAME}_FOUND)
    
    # message(WARNING "${THIS_FILE_LIB_NAME} not found. Run build_external to install it locally. Or install with your package manager.")
    message(STATUS "Compiling ${THIS_FILE_LIB_NAME} directly into the lib.")
    set(THIS_FILE_TARGET_NAME e_${THIS_FILE_LIB_NAME})
    
    #    set(_s3SOURCE_DIR ${PROJECT_CMK_EXT_PROJS_PRE}/${THIS_FILE_TARGET_NAME})
    #    set(_s3INSTALL_DIR ${PROJECT_CMK_EXT_PROJS_PRE})
    #    set(sqlite3_c_file ${_s3SOURCE_DIR}/sqlite3.c)
    #    set(sqlite3_h_file ${_s3SOURCE_DIR}/sqlite3.h)
    #    set(sqlite3_installed_include "${_s3INSTALL_DIR}/include")
    #    set(sqlite3_installed_h_file "${sqlite3_installed_include}/sqlite3.h")
    
    ExternalProject_Add(
        ${THIS_FILE_TARGET_NAME}
        PREFIX ${PROJECT_CMK_EXT_PROJS_PRE}
        URL https://sqlite.org/2019/sqlite-amalgamation-${DUTILCPPOPT_SQLITE3_VERSION}.zip
        CONFIGURE_COMMAND ""
        # BUILD_COMMAND "${CMAKE_COMMAND}" RENAME "${sqlite3_h_file}" "${sqlite3_installed_h_file}"
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        EXCLUDE_FROM_ALL TRUE
    )
    # print_external_itm_many(${THIS_FILE_TARGET_NAME})
    # ExternalProject_Add_Step(${THIS_FILE_TARGET_NAME} sql3_mv COMMAND "${CMAKE_COMMAND}" RENAME "${sqlite3_h_file}" "${sqlite3_installed_h_file}")
    ExternalProject_Get_Property(${THIS_FILE_TARGET_NAME} SOURCE_DIR)
    set(sqlite3_c_file ${SOURCE_DIR}/sqlite3.c)
    
    set_source_files_properties("${sqlite3_c_file}" PROPERTIES GENERATED TRUE)
    target_sources(dCppUtilsLib PRIVATE ${sqlite3_c_file})
    target_include_directories(dCppUtilsLib PUBLIC ${SOURCE_DIR})
    target_compile_definitions(dCppUtilsLib PUBLIC SQLITE_THREADSAFE=1 SQLITE_OMIT_LOAD_EXTENSION=1)
    foreach (c SQLITE_ENABLE_FTS3 SQLITE_ENABLE_FTS3_PARENTHESIS SQLITE_ENABLE_FTS4 SQLITE_ENABLE_RTREE SQLITE_ENABLE_COLUMN_METADATA)
      if (${c})
        target_compile_definitions(dCppUtilsLib PUBLIC ${c})
      endif ()
    endforeach ()
    
    add_dependencies(dCppUtilsLib ${THIS_FILE_TARGET_NAME})
    add_dependencies(build_external ${THIS_FILE_TARGET_NAME})
    if (${CMK_SCRIPT_VERBOSE})
      print_external_itm_many(${THIS_FILE_TARGET_NAME})
    endif ()
  else ()
    if (${CMK_SCRIPT_VERBOSE})
      message(STATUS "Found ${THIS_FILE_LIB_NAME}: ${${THIS_FILE_LIB_NAME}_DIR}")
      print_target_properties(sqlite3::sqlite3)
    endif ()
    target_link_libraries(dCppUtilsLib PUBLIC sqlite3::sqlite3)
  endif ()
elseif (${CMK_SCRIPT_VERBOSE})
  message(STATUS "Protobuf include disabled for: ${CMAKE_CURRENT__s3SOURCE_DIR}")
endif ()