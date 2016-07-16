cmake_minimum_required(VERSION 2.6.4)

# read in configuration file
function(read_configuration_file file_name)
    file(STRINGS ${file_name} FILE_CONTENTS)
    string(REGEX MATCHALL "([^ {}]+ [^ {}]+)" VariableList "${FILE_CONTENTS}")
    message(STATUS "Loading configuration file...\n")
    foreach (NameAndValue ${VariableList})
        #    message(STATUS "NameAndValue = ${NameAndValue}")
        # Find variable name
        string(REGEX MATCH "^[^ ]+" Name ${NameAndValue})
        # Find the value
        string(REGEX MATCH "[^ ]+$" Value ${NameAndValue})
        # Set the variable
        set(${Name} "${Value}" PARENT_SCOPE)
        message(STATUS "{${Name} : ${Value}}")
    endforeach ()
endfunction(read_configuration_file)

read_configuration_file("config.txt")

if(IN_CYGWIN)
#    message(status "Set cmake legacy setting")
    set(CMAKE_LEGACY_CYGWIN_WIN32 0)
#else()
#    message(status "Flag not set")
endif()

include_directories(".")

add_subdirectory(lib)
add_subdirectory(test)

project(${PROJECT_NAME})

include_directories(${${PROJECT_LIBS_NAME}_SOURCE_DIR}/include
        ${${PROJECT_LIBS_NAME}_SOURCE_DIR})

add_executable(${PROJECT_NAME} vmsim.c)
target_link_libraries(${PROJECT_NAME} ${PROJECT_LIBS_NAME})

#add_executable(Single_${PROJECT_NAME} main_single.c library.c library.h iso_font.h)

