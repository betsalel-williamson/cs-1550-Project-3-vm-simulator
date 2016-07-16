#! /usr/bin/env tclsh8.5

source common.tcl

if {[file isdirectory $MAIN_EXE]} {
  # this is a directory
} else {
  # not a directory
  exec $BUILD_SCRIPT_FILE
}

if { [catch {set result [exec $TEST_SCRIPT_FILE]} reason] } {

    puts "Failed tests: $reason"

} else {

    if { [catch {set result [exec $MAIN_EXE]} reason] } {

    puts "Failed main execution: $reason"

    } else {

    puts $result

    }
#}

