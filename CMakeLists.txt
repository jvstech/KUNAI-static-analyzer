cmake_minimum_required(VERSION 3.21)

project(kunai-static-analyzer)

find_package(Git QUIET)
if (GIT_FOUND AND EXISTS ".git")
  # Update submodules as needed.
  option(UPDATE_SUBMODULES "Check submodules during build" ON)
  if (UPDATE_SUBMODULES)
    message(STATUS "Performing submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      RESULT_VARIABLE UPDATE_SUBMODULES_RESULT)
    if (NOT UPDATE_SUBMODULES_RESULT EQUAL "0")
      message(FATAL_ERROR
        "Submodule update failed with ${UPDATE_SUBMODULES_RESULT}; please checkout"
        "submodules.")
    endif()
  endif()
endif()

macro(verify_submodule name)
  if (NOT EXISTS "${PROJECT_SOURCE_DIR}/kunai-lib/externals/${name}/CMakeLists.txt")
    message(FATAL_ERROR
      "The ${name} submodule was not downloaded. UPDATE_SUBMODULES was turned off or"
      "failed. Please update submodules and try again.")
  endif()
endmacro()

verify_submodule(spdlog)
verify_submodule(zip)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} cmake)
include(KUNAI)

add_subdirectory(kunai-lib)
add_subdirectory(dalvik-disassembler)

# TODO