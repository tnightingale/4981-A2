#! /bin/bash

os=`uname`
command="./client tmp.txt"

for ((i=$1; i > 0; i = i - 2)); do
    echo "Starting client, priority: $i"
    if [[ $os == 'Linux' ]]; then
      gnome-terminal -e "$command $i"
    elif [[ $os == 'Darwin' ]]; then
      appletermexec "$command $1"
    fi
    sleep $2
  done

function appletermexec {   
  osascript <<END 
  tell application "Terminal"
    do script "cd \"`pwd`\";$1;exit"
  end tell
  END
}
