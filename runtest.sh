#!/bin/bash

os=`uname`
command="./client tmp.txt"
i=$1

while [ $i -gt 0 ]; do
  echo "Starting client, priority: $i"

  if [[ $os == 'Linux' ]]; then
    gnome-terminal -e "$command $i"
  elif [[ $os == 'Darwin' ]]; then
    echo "Mac OS"
    ./term -x "$command $i"
  fi

  sleep $2
  let i-=2
done

