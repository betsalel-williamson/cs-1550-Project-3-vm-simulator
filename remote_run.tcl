#! /usr/bin/env expect

## note that the project must already exist on the remote server for this to run

## passwords are stored on local computer and outside the project directory to protect information
## TODO: switch to alternate method of password storage and retreval using SSH keys


source common.tcl
source $SECRET_FILE

set timeout 20

set server "thoth"
set ip "$server.cs.pitt.edu"

spawn ssh "$user@$ip"

expect \
    "password:" { send "$pass\r" }

expect \
    "$server" { send "cd /u/OSLab/$user/$git_project_name\r" }

expect \
    "$server" { send "git pull\r" }

expect \
    "$server" { send "./run.tcl\r" }

expect \
    "$server" { send "exit\r" }

interact


