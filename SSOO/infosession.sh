#!/bin/bash

CORRECT_PARAMS=3

show_process() {
  ps --no-header -eo sid,pgid,pid,user,tty,%mem,cmd --sort=user | awk '$1 != 0'
}

check_args() {
  if [ "$#" -gt "$CORRECT_PARAMS" ]; then
    echo "Incorrect number of params, please introduce a maximum of $CORRECT_PARAMS params"
    exit 1
  fi
}

# Llamar a check_args antes de mostrar los procesos
check_args "$@"

cat << _EOF_
$(show_process)
_EOF_
