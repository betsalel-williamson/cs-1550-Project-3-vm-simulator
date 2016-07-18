#! /usr/bin/env expect

#/*
# * MIT License
# *
# * Copyright (c) 2016 Betsalel "Saul" Williamson
# *
# * contact: saul.williamson@pitt.edu
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
#//  8, 16, 32, and 64
set OPTIONS { {-n 8 -a opt $TRACE_FILE_LOCATION}
   {-n 16 -a opt $TRACE_FILE_LOCATION}
   {-n 32 -a opt $TRACE_FILE_LOCATION}
   {-n 64 -a opt $TRACE_FILE_LOCATION}
   {-n 8 -a lru $TRACE_FILE_LOCATION}
   {-n 16 -a lru $TRACE_FILE_LOCATION}
   {-n 32 -a lru $TRACE_FILE_LOCATION}
   {-n 64 -a lru $TRACE_FILE_LOCATION}
}

#set OPTIONS { {-n 8 -a aging -r 50 $TRACE_FILE_LOCATION}
#       {-n 16 -a aging -r 50 $TRACE_FILE_LOCATION}
#       {-n 32 -a aging -r 50 $TRACE_FILE_LOCATION}
#       {-n 64 -a aging -r 50 $TRACE_FILE_LOCATION}
#       {-n 8 -a aging -r 500 $TRACE_FILE_LOCATION}
#       {-n 16 -a aging -r 500 $TRACE_FILE_LOCATION}
#       {-n 32 -a aging -r 500 $TRACE_FILE_LOCATION}
#       {-n 64 -a aging -r 500 $TRACE_FILE_LOCATION}
#       {-n 8 -a aging -r 5000 $TRACE_FILE_LOCATION}
#       {-n 16 -a aging -r 5000 $TRACE_FILE_LOCATION}
#       {-n 32 -a aging -r 5000 $TRACE_FILE_LOCATION}
#       {-n 64 -a aging -r 5000 $TRACE_FILE_LOCATION}
#       {-n 8 -a aging -r 50000 $TRACE_FILE_LOCATION}
#       {-n 16 -a aging -r 50000 $TRACE_FILE_LOCATION}
#       {-n 32 -a aging -r 50000 $TRACE_FILE_LOCATION}
#       {-n 64 -a aging -r 50000 $TRACE_FILE_LOCATION}
#       {-n 8 -a clock -r 50 $TRACE_FILE_LOCATION}
#       {-n 16 -a clock -r 50 $TRACE_FILE_LOCATION}
#       {-n 32 -a clock -r 50 $TRACE_FILE_LOCATION}
#       {-n 64 -a clock -r 50 $TRACE_FILE_LOCATION}
#       {-n 8 -a clock -r 500 $TRACE_FILE_LOCATION}
#       {-n 16 -a clock -r 500 $TRACE_FILE_LOCATION}
#       {-n 32 -a clock -r 500 $TRACE_FILE_LOCATION}
#       {-n 64 -a clock -r 500 $TRACE_FILE_LOCATION}
#       {-n 8 -a clock -r 5000 $TRACE_FILE_LOCATION}
#       {-n 16 -a clock -r 5000 $TRACE_FILE_LOCATION}
#       {-n 32 -a clock -r 5000 $TRACE_FILE_LOCATION}
#       {-n 64 -a clock -r 5000 $TRACE_FILE_LOCATION}
#       {-n 8 -a clock -r 50000 $TRACE_FILE_LOCATION}
#       {-n 16 -a clock -r 50000 $TRACE_FILE_LOCATION}
#       {-n 32 -a clock -r 50000 $TRACE_FILE_LOCATION}
#       {-n 64 -a clock -r 50000 $TRACE_FILE_LOCATION}
#}

set chan [open run.log a]
set timestamp [clock format [clock seconds]]
puts $chan "$timestamp\n"

foreach option $OPTIONS {

    if { [catch {set result [exec $MAIN_EXE {*}[eval list $option]]} reason] } {

    puts "Failed main execution: $reason"

    } else {

    puts $chan $result

    }
}

puts $chan "\n"

close $chan
