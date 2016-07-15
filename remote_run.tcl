#! /usr/bin/env expect

source /Users/school/pass.tcl

set timeout 20

set ip "thoth.cs.pitt.edu"

spawn ssh "$user@$ip"
expect "password:" {send "$pass\r" }

expect "" {send "cd /u/OSLab/$user/cs-1550-Project-3-vm-simulator\r"}
expect "" {send "git pull\r"}
expect "" {send "./run.tcl\r"}

interact


