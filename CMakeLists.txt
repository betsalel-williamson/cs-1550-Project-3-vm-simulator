cmake_minimum_required(VERSION 2.6.4)

set(CMAKE_BUILD_TYPE Debug)

file(STRINGS "config.txt" FILE_CONTENTS)
string(REGEX MATCHALL "([^ {}]+ [^ {}]+)" VariableList "${FILE_CONTENTS}")
message(STATUS "Loading configuration file...\n")
foreach (NameAndValue ${VariableList})
    #    message(STATUS "NameAndValue = ${NameAndValue}")
    # Find variable name
    string(REGEX MATCH "^[^ ]+" Name ${NameAndValue})
    # Find the value
    string(REGEX MATCH "[^ ]+$" Value ${NameAndValue})
    # Set the variable
    set(${Name} "${Value}")
    message(STATUS "{${Name} : ${Value}}")
endforeach ()

include_directories(".")

add_subdirectory(lib)
add_subdirectory(test)

project(${PROJECT_NAME})

include_directories(${ProjectLibs_SOURCE_DIR}/include
        ${ProjectLibs_SOURCE_DIR})

add_executable(${PROJECT_NAME} vmsim.c)
target_link_libraries(${PROJECT_NAME} ProjectLibs)

#add_executable(Single_${PROJECT_NAME} main_single.c library.c library.h iso_font.h)



