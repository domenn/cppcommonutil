if (DUTILCPP_ADD_CLI11)
  set(THE_FILE_LIB_NAME e_cli11)
  set(THE_FILE_LIB_VERSION v1.8.0)
  
  ExternalProject_Add(
      ${THE_FILE_LIB_NAME}
      PREFIX ${PROJECT_CMK_EXT_PROJS_PRE}
      URL https://github.com/CLIUtils/CLI11/releases/download/${THE_FILE_LIB_VERSION}/CLI11.hpp
      DOWNLOAD_NO_EXTRACT 1
      DOWNLOAD_DIR ${PROJECT_CMK_EXT_PROJS_PRE}/include
      CONFIGURE_COMMAND ""
      BUILD_COMMAND ""
      INSTALL_COMMAND ""
      EXCLUDE_FROM_ALL TRUE
  )

  ##### print_external_itm_many(${THE_FILE_LIB_NAME})
  add_dependencies(build_external ${THE_FILE_LIB_NAME})
  ExternalProject_Get_Property(${THE_FILE_LIB_NAME} INSTALL_DIR)
  target_include_directories(dCppUtilsLib PUBLIC ${INSTALL_DIR}/include)
endif ()