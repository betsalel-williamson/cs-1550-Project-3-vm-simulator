include_directories(${gtest_SOURCE_DIR}/include
        ${gtest_SOURCE_DIR}
        ${${PROJECT_LIBS_NAME}_SOURCE_DIR}/include
        ${${PROJECT_LIBS_NAME}_SOURCE_DIR})

add_executable(${TEST_SUITE_NAME}
        mother.cpp
        library_tests.cpp)

target_link_libraries(${TEST_SUITE_NAME} ${PROJECT_LIBS_NAME} gtest gtest_main)

add_test(NAME ${TEST_SUITE_NAME}
        COMMAND ${TEST_SUITE_NAME})