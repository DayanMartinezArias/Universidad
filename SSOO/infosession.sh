#!/bin/bash

PROGNAME=$(basename $0)
USERS=""
ROUTE=""
PIDZERO="no"

show_process() {
  if [ -z "$USERS" ]; then
    ps ax -o sid,pgid,pid,user,tty,%mem,cmd --sort=user | awk '$1 != 0'
  else
    ps ax -o sid,pgid,pid,user,tty,%mem,cmd --sort=user | awk -v user="$USERS" '$4 == user && $1 != 0'
  fi
}

show_process_sid_zero() {
  if [ -z "$USERS" ]; then
    ps ax -o sid,pgid,pid,user,tty,%mem,cmd --sort=user
  else
    ps ax -o sid,pgid,pid,user,tty,%mem,cmd --sort=user | awk -v user="$USERS" '$4 == user'
  fi
}

show_help() {
  cat << _EOF_
Usage: $PROGNAME [OPTIONS]

Options:
  -h        Show this help message and exit
  -z        Shows all processes with sid 0
  -u user   Shows all processes of a specific user
_EOF_
}

unknown_param() {
  echo "ERROR: Unknown parameter introduced: $1"
}

check_help() {
  for arg in "$@"; do
    if [ "$arg" == "-h" ]; then
      show_help
      exit 0
    fi
  done
}

read_args() {
  while [ "$1" != "" ]; do
    case $1 in
      -u )
        shift
        if [ -z "$1" ]; then
          echo "Option -u requires an argument"
          exit 1
        fi
        if [ -n "$USERS" ]; then
          unknown_param "$1"
          exit 1
        fi        
        USERS=$1
        if ! grep -q "^$USERS:" /etc/passwd; then
          echo "ERROR: User '$USERS' not found"
          exit 1
        fi
        ;;
      -d )
        shift
        if [ -z "$1" ]; then
          echo "Option -d requires an argument"
          exit 1
        fi
        if [ -n "$ROUTE" ]; then
          unknown_param "$1"
          exit 1
        fi        
        ROUTE=$1
        if [ -d "$ROUTE" ]; then 
          lsof +d "$ROUTE"
        else
          echo "Directory does not exist" 
          exit 1
        fi
        ;;
      -z )
        if [ "$PIDZERO" == "no" ]; then 
          PIDZERO="yes"
        else 
          unknown_param "$1"
          exit 1
        fi
        ;;
      * )
        unknown_param "$1"
        exit 1
    esac
    shift
  done
}

check_help "$@"
read_args "$@" 

if [ "$PIDZERO" == "no" ]; then
  show_process
  exit 0
else
  show_process_sid_zero
  exit 0
fi


