#! /usr/bin/env expect
source /Users/school/pass.tcl

set location [lindex $argv 0];

set timeout 20

set ip "thoth.cs.pitt.edu"

spawn scp "$user@$ip:$location" downloads
expect "password:" {send "$pass\r" }

interact
