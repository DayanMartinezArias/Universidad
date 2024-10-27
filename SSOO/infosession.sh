#!/bin/bash

# Universidad de la Laguna
# Sistemas Operativos
# Dayán Martínez Arias 
# alu0101644561
# PE102
# 27/10/24

PROGNAME=$(basename $0)
USERS=""
ROUTE=""
PIDZERO=false

if ! command -v ps 2>&1 >/dev/null ||  ! command -v awk 2>&1 >/dev/null ||  ! command -v lsof 2>&1 >/dev/null; then
  echo "ERROR: This script requires to have awk, lsof and ps installed on your computer"
  exit 1
fi

get_ps() {
  ps ax -o sid,pgid,pid,user,tty,%mem,cmd --sort=user
}

show_process() {
  local ps_command
  ps_command=$(get_ps)
  
  if [ -z "$USERS" ]; then
    echo "$ps_command" | awk '$1 != 0' | column -t
  else
    echo "$ps_command" | awk -v user="$USERS" '$4 == user && $1 != 0' | column -t
  fi
}

show_process_with_directory() {
  local pids
  pids=$(lsof -t +d "$ROUTE" | tr '\n' ',')
  
  if [ -n "$pids" ]; then
    ps -p "${pids%,}" -o sid,pgid,pid,user,tty,%mem,cmd --sort=user | column -t
  else
    echo "No processes are accessing the specified directory."
  fi
}

show_process_sid_zero() {
  local ps_command
  ps_command=$(get_ps)

  if [ -z "$USERS" ]; then
    echo "$ps_command" | column -t
  else
    echo "$ps_command" | awk -v user="$USERS" '$4 == user' | column -t
  fi
}

show_help() {
  cat << _EOF_
Usage: $PROGNAME [OPTIONS]

Options:
  -h        Show this help message and exit
  -z        Shows all processes with sid 0
  -u user   Shows all processes of a specific user
  -d dir    Shows all processes accessing a specific directory
_EOF_
}

unknown_param() {
  echo "ERROR: Unknown parameter introduced: $1"
}

double_param() {
  echo "ERROR: Double option was introduced"
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
          double_param "$1"
          exit 1
        fi        
        USERS=$1
        if ! id -u "$USERS" >/dev/null 2>&1; then
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
          double_param "$1"
          exit 1
        fi        
        ROUTE=$1
        if [ ! -d "$ROUTE" ]; then
          echo "ERROR: Directory does not exist" 
          exit 1
        fi
        ;;
      -z )
        if [ "$PIDZERO" == false ]; then 
          PIDZERO=true
        else 
          double_param "$1"
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

if [ -n "$ROUTE" ]; then
  show_process_with_directory
elif [ "$PIDZERO" == true ]; then
  show_process_sid_zero
else
  show_process
fi
