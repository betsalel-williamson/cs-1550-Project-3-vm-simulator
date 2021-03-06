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
    "$server" { send "cat test.log\r" }

expect \
    "$server" { send "exit\r" }

interact


