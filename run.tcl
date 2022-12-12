#! /usr/bin/env expect

#/*
# * MIT License
# *
# * Copyright (c) 2016 Betsalel "Saul" Williamson
# *
# * contact: saul.williamson@ieee.org
# *
# * Permission is hereby granted, free of charge, to any person obtaining a copy
# * of this software and associated documentation files (the "Software"), to deal
# * in the Software without restriction, including without limitation the rights
# * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# * copies of the Software, and to permit persons to whom the Software is
# * furnished to do so, subject to the following conditions:
# *
# * The above copyright notice and this permission notice shall be included in all
# * copies or substantial portions of the Software.
# *
# * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# * SOFTWARE.
# */

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

puts $TRACE_FILE_LOCATION
# start of end to end tests
# need to run  multiple scenarios
#./vmsim –n <numframes> -a <opt|clock|aging|lru> [-r <refresh>] <tracefile>

set numFrames {8 16 32 64}
set algorithms {opt clock aging lru}
set refreshRates {50 500 5000 50000}

# create args from numFrames, algorithms, refreshRates
set ARGS {}

foreach numFrame $numFrames {
  foreach algorithm $algorithms {
    switch $algorithm {
      clock - aging {
        foreach refreshRate $refreshRates {
            lappend ARGS "-n $numFrame -a $algorithm -r $refreshRate $TRACE_FILE_LOCATION"
        }
      }
      default {
            lappend ARGS "-n $numFrame -a $algorithm $TRACE_FILE_LOCATION"
      }
    }
  }
}

set chan [open run.log a]
set timestamp [clock format [clock seconds]]
puts $chan "$timestamp\n"

foreach args $ARGS {
    puts "running: $MAIN_EXE [eval list $args]"
    if { [catch {set result [exec $MAIN_EXE {*}[eval list $args]]} reason] } {
      puts "Failed main execution: $reason"
    } else {
      puts "Success!"
      puts $chan $result
    }
}

puts $chan "\n"

close $chan
