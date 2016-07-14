#! /usr/bin/env tclsh8.5

proc cf_read fname {
  set fd [open $fname]
  chan config $fd -encoding utf-8
  set data [read $fd]
  close $fd
  set data
}

proc cf_write {fname args} {
  set fd [open $fname w]
  chan config $fd -encoding utf-8
  set data [list]
  foreach varName $args {
    upvar 1 $varName var
    lappend data [list $varName $var]
  }
  puts $fd $data
  close $fd
}

set cfile [file join [file dir [info script]] config.txt]

if {[file exists config.txt]} {
  # this is a directory
} else {
  # not a directory
    set BUILD_DIR _build

    set PROJECT_NAME vmsim

    set TEST_SUITE_NAME TestSuite

    set BUILD_SCRIPT_FILE ./build.tcl

    set TEST_SCRIPT_FILE ./test.tcl

    set TEST_EXE $BUILD_DIR/test/mainTests/$TEST_SUITE_NAME

    set MAIN_EXE $BUILD_DIR/$PROJECT_NAME

  cf_write $cfile BUILD_DIR PROJECT_NAME TEST_SUITE_NAME BUILD_SCRIPT_FILE TEST_SCRIPT_FILE TEST_EXE MAIN_EXE
}

puts "Reading in configuation file..."
foreach entry [cf_read $cfile] {
  lassign $entry name value
  set $name $value
#  set
  puts "$name: $value"
}
