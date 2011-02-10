#!/bin/bash

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

