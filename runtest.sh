#!/bin/bash

##
# COMP 4981 - Assignment 2; Interprocess Communication, Message queues.
## 
# Testing script
# @author Tom Nightingale
#
# This script is designed to startup upto 5 clients with a specified delay between 
# each creation. The first client will have the lowest priority and the last will 
# have the highest. The purpose of this test is to demonstrate the server's
# priority scheduling is functional.
#
##
# Usage
# ./runtest.sh filepath lowest_priority seconds_to_sleep
# 
# Notes:  
#   - filepath must point to a file available on the system.
#   - lowest_priority must be a number lower than or equal to 9.
#   - seconds_to_sleep is the number of seconds to sleep between each client 
#     creation
#

os=`uname`
command="./client $1"
i=$2

while [ $i -gt 0 ]; do
  echo "Starting client, priority: $i"

  if [[ $os == 'Linux' ]]; then
    gnome-terminal -e "$command $i"
  elif [[ $os == 'Darwin' ]]; then
    echo "Mac OS"
    ./term "$command $i"
  fi

  sleep $3
  let i-=2
done

