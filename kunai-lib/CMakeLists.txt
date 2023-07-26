cmake_minimum_required(VERSION 3.5)

# Information from Kunai project
project(
    Kunai
    LANGUAGES CXX
    VERSION 2.5.0
    DESCRIPTION "Tool for static analysis of DEX files with the chance to lift and analyze with an Intermediate Representation"
    HOMEPAGE_URL https://github.com/Fare9/KUNAI-static-analyzer
)


# options to have variable and message
option(UNIT_TESTING "Compile the binaries for unit tests" OFF)
option(GITHUB_ACTION "Compilation is done into github action" OFF)
option(USE_MJOLNIR "Compile Kunai's IR with LLVM (you must have LLVM installed)" OFF)

kunai_show_compiler_info()

# include the externals
add_subdirectory(externals)

# Kunai inlcude Definition
set(KUNAI_LIB_PUBLIC_INCLUDE_FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/include/"
)

message(STATUS "Generating Kunai shared library")

set_target_properties(spdlog
    PROPERTIES
    POSITION_INDEPENDENT_CODE 1
)

set_target_properties(zip
    PROPERTIES
    POSITION_INDEPENDENT_CODE 1
)

message(STATUS "Generating object files")

# compile the object files
add_library(kunai-objs
    OBJECT
)

# include the CMAKEs from each
add_subdirectory(lib)

target_link_libraries(kunai-objs PUBLIC spdlog zip)

# TODO: Handle SOURCE_DIR for kunai-objs to automatically add the include directory

# include directories for Kunai
include_directories(BEFORE

    # ${CMAKE_CURRENT_BINARY_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${spdlog_SOURCE_DIR}/include/
    ${zip_SOURCE_DIR}/src/
)

set_target_properties(kunai-objs
    PROPERTIES
    POSITION_INDEPENDENT_CODE 1
)

set(TARGET_INCLUDE_LIBS
    spdlog
    zip
)

set(TARGET_LINK_DIRECTORIES)

# #########################################
# In case use MjolnIR with MLIR
if(USE_MJOLNIR)
    find_package(MLIR REQUIRED CONFIG)

    message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

    set(LLVM_RUNTIME_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/bin)
    set(LLVM_LIBRARY_OUTPUT_INTDIR ${CMAKE_BINARY_DIR}/lib)
    set(MLIR_BINARY_DIR ${CMAKE_BINARY_DIR})

    list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
    list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

    include(TableGen)
    include(AddLLVM)
    include(AddMLIR)

    include_directories(${LLVM_INCLUDE_DIRS})
    include_directories(${MLIR_INCLUDE_DIRS})
    include_directories(${PROJECT_BINARY_DIR}/MjolnIR/include)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/MjolnIR/include/)
    include_directories(${CMAKE_CURRENT_LIST_DIR}/MjolnIR/lib/Dalvik/)
    link_directories(${LLVM_BUILD_LIBRARY_DIR})
    add_definitions(${LLVM_DEFINITIONS})

    add_subdirectory(MjolnIR)

    set_target_properties(Lifter
        PROPERTIES
        POSITION_INDEPENDENT_CODE 1
    )

    # include TARGET_LINK_DIRECTORIES in case
    list(APPEND TARGET_LINK_DIRECTORIES
        ${PROJECT_BINARY_DIR}/lib/
        ${PROJECT_BINARY_DIR}/MjolnIR/lib/Lifter/
    )

    # include TARGET_INCLUDE_LIBS the Lifter and MjolnIR dialect
    list(APPEND TARGET_INCLUDE_LIBS
        MjolnIR
        Lifter
    )
endif()

add_library(KUNAI SHARED
    $<TARGET_OBJECTS:kunai-objs>
)

target_link_directories(KUNAI
    PUBLIC
    ${TARGET_LINK_DIRECTORIES}
)

target_link_libraries(KUNAI
    PUBLIC
    ${TARGET_INCLUDE_LIBS}
)

# target properties for shared object
set_target_properties(KUNAI
    PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    OUTPUT_NAME "kunai"
)

if(NOT USE_MJOLNIR)
    # ######################################################################
    # installation
    message(STATUS "Project will be installed to ${CMAKE_INSTALL_PREFIX}")

    # Offer the user the choice of overriding the installation directories
    set(INSTALL_LIBDIR ${CMAKE_INSTALL_LIBDIR} CACHE PATH "Installation directory for libraries")
    set(INSTALL_INCLUDEDIR ${CMAKE_INSTALL_INCLUDEDIR} CACHE PATH "Installation directory for header files")

    # Report to user
    foreach(p LIB INCLUDE)
        file(TO_NATIVE_PATH ${CMAKE_INSTALL_PREFIX}/${INSTALL_${p}DIR} _path)
        message(STATUS "Installing ${p} components to ${_path}")
        unset(_path)
    endforeach()

    # install the libraries
    install(
        TARGETS KUNAI spdlog zip
        EXPORT KUNAITargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT libraries
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )

    install(
        DIRECTORY "${KUNAI_LIB_PUBLIC_INCLUDE_FILES}"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT headers
        FILES_MATCHING
        REGEX "(.*).(hpp|h|def|inc)$"
    )

    install(
        DIRECTORY "${spdlog_SOURCE_DIR}/include/"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
        COMPONENT headers
        FILES_MATCHING
        REGEX "(.*).(hpp|h|def|inc)$"
    )

    install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/KUNAIConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/KUNAIConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/KUNAI/cmake
        COMPONENT config
    )

    target_include_directories(KUNAI
        INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
    )

    install(
        EXPORT KUNAITargets
        FILE KUNAITargets.cmake
        DESTINATION lib/cmake/KUNAI
    )

    # Generate a package configuration file for other projects to use
    include(CMakePackageConfigHelpers)

    set(KUNAI_LIBRARY_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR})
    set(KUNAI_INCLUDE_DIR ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})

    message(STATUS "Exporting ${KUNAI_LIBRARY_DIR} and ${KUNAI_INCLUDE_DIR}")

    configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/KUNAIConfig.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/KUNAIConfig.cmake"
        INSTALL_DESTINATION "lib/cmake/KUNAI"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/KUNAIConfigVersion.cmake"
        VERSION "${KUNAI_VERSION_MAJOR}.${KUNAI_VERSION_MINOR}"
        COMPATIBILITY AnyNewerVersion
    )

    install(FILES
        ${CMAKE_CURRENT_BINARY_DIR}/KUNAIConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/KUNAIConfigVersion.cmake
        DESTINATION lib/cmake/KUNAI
    )

    export(EXPORT KUNAITargets
        FILE "${CMAKE_CURRENT_BINARY_DIR}/KUNAITargets.cmake"
    )

    export(PACKAGE KUNAI)
endif()

# ######################################################################
if(UNIT_TESTING)
    include(CTest)

    enable_testing()

    set(KUNAI_TEST_FOLDERS
        "${CMAKE_CURRENT_SOURCE_DIR}/tests"
    )
    add_subdirectory(unit-tests)
endif()