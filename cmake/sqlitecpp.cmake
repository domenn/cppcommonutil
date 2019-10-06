
if (DUTILCPP_ADD_SQLITECPP)
  
  INCLUDE(CheckIncludeFiles)
  
  option(SQLITECPP_RUN_CPPLINT "Run cpplint.py tool for Google C++ StyleGuide." OFF)
  option(SQLITECPP_RUN_CPPCHECK "Run cppcheck C++ static analysis tool." OFF)
  option(SQLITECPP_USE_GCOV "override" OFF)
  option(SQLITECPP_BUILD_EXAMPLES "override" OFF)
  option(SQLITECPP_BUILD_TESTS "over" OFF)
  # option(SQLITECPP_INTERNAL_SQLITE "sqlite3build" ON)
  option(SQLITE_ENABLE_COLUMN_METADATA "sqliteSmth" ON)
  
  set(THIS_FILE_TARGET_NAME e_sqlite3cpp)
  ExternalProject_Add(
      ${THIS_FILE_TARGET_NAME}
      PREFIX ${PROJECT_CMK_EXT_PROJS_PRE}
      GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git
      GIT_TAG 2.4.0
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      EXCLUDE_FROM_ALL TRUE
  )
  
  ExternalProject_Get_Property(${THIS_FILE_TARGET_NAME} SOURCE_DIR)
  set(${THIS_FILE_TARGET_NAME}_src_dir ${SOURCE_DIR})
  
  if (NOT EXISTS "${${THIS_FILE_TARGET_NAME}_src_dir}/CMakeLists.txt")
    message(WARNING "${THIS_FILE_TARGET_NAME} not downloaded. Run it's target or all external.")
  else ()
    # add_subdirectory("${${THIS_FILE_TARGET_NAME}_src_dir}" "${CMAKE_CURRENT_BINARY_DIR}/SqlitecppBuild" EXCLUDE_FROM_ALL)
    file(GLOB_RECURSE SQLITECPP_SRC_STR LIST_DIRECTORIES false "${${THIS_FILE_TARGET_NAME}_src_dir}/src/*.cpp*")
    target_sources(dCppUtilsLib PRIVATE ${SQLITECPP_SRC_STR})
    target_sources(dCppUtilsLib PRIVATE "${${THIS_FILE_TARGET_NAME}_src_dir}/sqlite3/sqlite3.c")

    find_package (Threads)
    
    target_link_libraries(dCppUtilsLib PRIVATE
        # SQLiteCpp
        # sqlite3
        ${CMAKE_THREAD_LIBS_INIT})
    #    message(WARNING "including: dCppUtilsLib PUBLIC \"${${THIS_FILE_TARGET_NAME}_src_dir}/include\"")
    
    CHECK_INCLUDE_FILES("sqlite3.h" SQLITE3_H_IN)
    if (NOT SQLITE3_H_IN)
      target_include_directories(dCppUtilsLib PUBLIC "${${THIS_FILE_TARGET_NAME}_src_dir}/sqlite3")
      CHECK_INCLUDE_FILES("sqlite3.h" SQLITE3_H_IN)
    endif ()
    
    target_include_directories(dCppUtilsLib PUBLIC "${${THIS_FILE_TARGET_NAME}_src_dir}/include")
  endif ()
  add_dependencies(build_external ${THIS_FILE_TARGET_NAME})
endif ()