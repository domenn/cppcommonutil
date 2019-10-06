#print_any_var(CMAKE_CURRENT_LIST_DIR)
#print_any_var(CMAKE_CURRENT_LIST_FILE)

# Idea: https://www.codeofclimber.ru/2016/using-protobuf-v2-in-cmake-projects-on-windows/

if (DUTILCPP_ADD_PROTOBUF)
    if (${CMK_SCRIPT_VERBOSE})
        include(${CMAKE_CURRENT_LIST_DIR}/util/get_target_props.cmake)
    endif ()
    set(PROJECT_CMK_EXT_PROJS_PRE_CUSTOM ${CMAKE_CURRENT_BINARY_DIR}/CmkExtProj)
    find_package(protobuf
            #        PATHS
            #        ${PROJECT_CMK_EXT_PROJS_PRE}/cmake
            HINTS
            ${PROJECT_CMK_EXT_PROJS_PRE_CUSTOM}
            NO_DEFAULT_PATH)

#    if (protobuf_FOUND)
#      #p rint_target_properties(protobuf::libprotobuf)
#      get_target_property(lpb_imp_conf protobuf::libprotobuf IMPORTED_CONFIGURATIONS)
#      # message(STATUS "${CMAKE_BUILD_TYPE} IN_LIST ${lpb_imp_conf}")
#      if ("${CMAKE_BUILD_TYPE}" IN_LIST lpb_imp_conf)
#        if (${CMK_SCRIPT_VERBOSE})
#          message(STATUS "Protobuf configs OK: ${lpb_imp_conf}")
#        endif ()
#        # set(IMPORT_DBG_REL_OK "1")
#      else()
#        # set(IMPORT_DBG_REL_OK "")
#        message(WARNING "Protobuf missing configuration (found configs: ${lpb_imp_conf}) You must force rebuild it.")
#        set(protobuf_FOUND "")
#        set(protobuf_DIR "")
#      endif()
#    endif()

    if (NOT protobuf_FOUND)

        message(WARNING "Protobuf not found. Run build_external to install it locally. Or install with your package manager.")

        set(THE_FILE_LIB_NAME e_protobuf)
        set(THE_FILE_LIB_VERSION v3.9.1)

        include(ExternalProject)

        ExternalProject_Add(
                ${THE_FILE_LIB_NAME}
                PREFIX ${PROJECT_CMK_EXT_PROJS_PRE_CUSTOM}
                GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
                GIT_TAG ${THE_FILE_LIB_VERSION}
                GIT_SHALLOW 1
                CMAKE_CACHE_ARGS
                "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
                "-Dprotobuf_BUILD_TESTS:BOOL=OFF"
                "-Dprotobuf_BUILD_SHARED_LIBS:BOOL=OFF"
                "-Dprotobuf_MSVC_STATIC_RUNTIME:BOOL=OFF"
                "-Dprotobuf_BUILD_EXAMPLES:BOOL=OFF"
                "-Dprotobuf_WITH_ZLIB:BOOL=OFF"
                "-DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}"
                "-DCMAKE_INSTALL_PREFIX:PATH=${PROJECT_CMK_EXT_PROJS_PRE_CUSTOM}"
                # other project specific parameters
                SOURCE_SUBDIR cmake
                # BUILD_ALWAYS 1
                # STEP_TARGETS build
                EXCLUDE_FROM_ALL true
        )

        add_dependencies(build_external ${THE_FILE_LIB_NAME})
        if (${CMK_SCRIPT_VERBOSE})
            print_external_itm_many(${THE_FILE_LIB_NAME})
        endif ()

        ExternalProject_Get_Property(${THE_FILE_LIB_NAME} SOURCE_DIR)

        if (${CMK_SCRIPT_VERBOSE})
            message(STATUS "Protobuf source: ${SOURCE_DIR}")
        endif ()

    else ()
        if (${CMK_SCRIPT_VERBOSE})
            message(STATUS "Found protobuf: ${protobuf_DIR}")

            print_target_properties(protobuf::protoc)
            print_target_properties(protobuf::libprotobuf)
        endif ()

        target_link_libraries(${PROTOBUF_FILE_INPUT_TARGET_NAME} PRIVATE protobuf::libprotobuf)

    endif ()
elseif (${CMK_SCRIPT_VERBOSE})
    message(STATUS "Protobuf include disabled for: ${CMAKE_CURRENT_SOURCE_DIR}")
endif ()