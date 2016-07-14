cmake_minimum_required(VERSION 2.6.4)

set(CMAKE_MACOSX_RPATH 1)

# Define helper functions and macros used by Google Test.
#include(../test/lib/googletest/cmake/internal_utils.cmake)

project(ProjectLibs CXX C)

#config_compiler_and_linker()  # Defined in internal_utils.cmake.

# Where Project_1's .h files can be found.
include_directories(
        ${ProjectLibs_SOURCE_DIR}/include
        ${ProjectLibs_SOURCE_DIR})

# Where Project_1's libraries can be found.
link_directories(${ProjectLibs_BINARY_DIR}/src)

########################################################################
#
# Defines the lib libraries.  User tests should link
# with one of them.

# Project_1 libraries.  We build them using more strict warnings than what
# are used for other targets, to ensure that gtest can be compiled by a user
# aggressive about warnings.
add_library(ProjectLibs STATIC
        src/controller.c
        include/controller.h
        src/view.c
        include/view.h
        src/model.c
        include/model.h)

SET_TARGET_PROPERTIES(ProjectLibs PROPERTIES LINKER_LANGUAGE C)

#cxx_library(Project_1_lib "${cxx_strict}" src/syscalls.c)
#target_link_libraries(Project_1_lib)