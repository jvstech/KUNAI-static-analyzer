function(kunai_configure_target name)
  set_target_properties(${name}
    PROPERTIES
      # Use C++20
      CXX_STANDARD 20
      CXX_STANDARD_REQUIRED TRUE)

  # for debug configuration set specific flags
  if (NOT WIN32)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
      target_compile_options(${name} PRIVATE -g -O0 -DDEBUG)
    elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
      target_compile_options(${name} PRIVATE -g -O0 -DNDEBUG)
    endif()
  endif()
endfunction()

function(kunai_show_compiler_info)
  # information about the compiler loaded
  # its ID and version
  if (CMAKE_CXX_COMPILER_LOADED)
    message(STATUS "C++ compiler ID: ${CMAKE_CXX_COMPILER_ID}")
    message(STATUS "C++ compiler version: ${CMAKE_CXX_COMPILER_VERSION}")
  endif()

  message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")

  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "C++ flags, Release configuration: ${CMAKE_CXX_FLAGS_RELEASE}")
  elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "C++ flags, Debug configuration: ${CMAKE_CXX_FLAGS_DEBUG}")
  elseif (CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    message(STATUS "C++ flags, Release configuration with Debug info: ${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
  elseif (CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
    message(STATUS "C++ flags, minimal Release configuration: ${CMAKE_CXX_FLAGS_MINSIZEREL}")
  endif()
endfunction()
