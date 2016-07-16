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
}

# start of end to end tests
# need to run  multiple scenarios
#./vmsim –n <numframes> -a <opt|clock|aging|lru> [-r <refresh>] <tracefile>
#//  8, 16, 32, and 64
set OPTIONS {
    {-n 8 -a opt $TRACE_FILE_LOCATION}
#    {-n 16 -a opt $TRACE_FILE_LOCATION}
#    {-n 32 -a opt $TRACE_FILE_LOCATION}
#    {-n 64 -a opt $TRACE_FILE_LOCATION}
#    {-n 8 -a clock -r 100 $TRACE_FILE_LOCATION}
#    {-n 16 -a clock -r 100 $TRACE_FILE_LOCATION}
#    {-n 32 -a clock -r 100 $TRACE_FILE_LOCATION}
#    {-n 64 -a clock -r 100 $TRACE_FILE_LOCATION}
#    {-n 8 -a aging -r 100 $TRACE_FILE_LOCATION}
#    {-n 16 -a aging -r 100 $TRACE_FILE_LOCATION}
#    {-n 32 -a aging -r 100 $TRACE_FILE_LOCATION}
#    {-n 64 -a aging -r 100 $TRACE_FILE_LOCATION}
#    {-n 8 -a lru $TRACE_FILE_LOCATION}
#    {-n 16 -a lru $TRACE_FILE_LOCATION}
#    {-n 32 -a lru $TRACE_FILE_LOCATION}
#    {-n 64 -a lru $TRACE_FILE_LOCATION}
}

set i 0
foreach option $OPTIONS {
    if { [catch {set result [exec $MAIN_EXE {*}$option]} reason] } {

    puts "Failed main execution: $reason"

    } else {

    puts $result

    }

    incr i
}

