cmake_minimum_required(VERSION 2.6.4)

set(CMAKE_MACOSX_RPATH 1)

# Define helper functions and macros used by Google Test.
#include(../test/lib/googletest/cmake/internal_utils.cmake)

project(${PROJECT_LIBS_NAME} CXX C)

#config_compiler_and_linker()  # Defined in internal_utils.cmake.

# Where Project_1's .h files can be found.
include_directories(
        ${${PROJECT_LIBS_NAME}_SOURCE_DIR}/include
        ${${PROJECT_LIBS_NAME}_SOURCE_DIR})

# Where Project_1's libraries can be found.
link_directories(${${PROJECT_LIBS_NAME}_BINARY_DIR}/src)

########################################################################
#
# Defines the lib libraries.  User tests should link
# with one of them.
#file(GLOB_RECURSE sources *.c)
#file(GLOB_RECURSE headers *.h)
#message(STATUS "sources = ${sources}")
#message(STATUS "headers = ${headers}")

# Project libraries.
add_library(${PROJECT_LIBS_NAME} STATIC
        include/model.h
        include/view.h
        include/controller.h

        src/model.c
        src/view.c
        src/controller.c)
#        ${headers}
#        ${sources})

SET_TARGET_PROPERTIES(${PROJECT_LIBS_NAME} PROPERTIES LINKER_LANGUAGE C)

#cxx_library(Project_1_lib "${cxx_strict}" src/syscalls.c)
#target_link_libraries(Project_1_lib)