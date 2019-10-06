

macro(write_single_item m_var)
  message(STATUS ${m_var}: ${${m_var}})
endmacro()

function(add_boost_stacktrace target_name)
  if (ARGV1 STREQUAL "VERBOSE")
    set(_thisFunctionVerbose "1")
  endif ()
  
  FIND_PACKAGE(Boost 1.65)
  if (Boost_FOUND)
    if (${_thisFunctionVerbose})
      message(STATUS "Found boost libs: ${Boost_LIBRARIES}")
      write_single_item(Boost_FOUND)
      write_single_item(Boost_INCLUDE_DIRS)
      write_single_item(Boost_LIBRARY_DIRS)
      write_single_item(Boost_LIBRARIES)
    endif ()
    target_compile_definitions(${target_name} PUBLIC D_HAS_BOOST_STACKTRACE)
    target_link_libraries(${target_name} Boost::boost ${CMAKE_DL_LIBS})
    target_compile_definitions(${target_name} PRIVATE BOOST_STACKTRACE_USE_ADDR2LINE)
  else ()
    message(STATUS "No boost, not linking stacktrace")
  endif ()

endfunction()
