//
// Created by Betsalel Williamson on 6/30/16.
//

#include <lib/include/controller.h>
#include "gtest/gtest.h"

TEST(Library_tests, Open_zeroth_framebuffer) {
    // opening this file will work

    int actual = 1; //foo();
//    int expected = 0;

//    printf("Opened filedesc: %d\n", filedesc);

    ASSERT_EQ(1, actual);
}

// ./vmsim –n <numframes> -a <opt|clock|aging|lru> [-r <refresh>] <tracefile>

// test num frames

// test opt

// test clock

// test aging

// test lru

// test refresh

// test read tracefile

TEST(Control_tests, Read_file_Test) {

    // open file
    read_trace_file(
            "/Users/school/Box Sync/Courses/- CS 1550/projects/3/cs-1550-Project-3-vm-simulator/downloads/gcc.trace");

    printf("Hello from inside read file test\n");
    //
}