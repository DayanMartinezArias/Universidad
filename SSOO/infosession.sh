#!/bin/bash

# Universidad de la Laguna
# Sistemas Operativos
# Dayán Martínez Arias 
# alu0101644561
# PE102
# PRIMERA ENTREGA: 30/10/24

# Style definitions
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'
BOLD=$(tput bold)
NORMAL=$(tput sgr0)

# Global variables
PROGNAME=$(basename $0)
USERS=""
ROUTE=""
SIDZERO=false
LINE="==========================================================="
WARNING=$(echo -e "${YELLOW}It is important to have awk, lsof, grep and ps previously installed!!${NC}")


# Check for requiered commands
check_commands() {
  if ! command -v ps >/dev/null 2>&1 || ! command -v awk >/dev/null 2>&1 || ! command -v lsof >/dev/null 2>&1 || ! command -v grep >/dev/null 2>&1; then
    show_error "This script requires awk, lsof, grep and ps installed on your computer"
    exit 1
  fi
}

# Function to show a help message
show_help() {
  cat << _EOF_
Usage: $PROGNAME [OPTIONS]

$WARNING

${BOLD}-h:${NORMAL}
Show this help message and exit

${BOLD}-z:${NORMAL}
Show all processes with SID 0 

${BOLD}-u user:${NORMAL}
Show all processes of a specific user-->
-A user must be introduced after the -u option-
-The user must be a real user on your computer-   

${BOLD}-d directory:${NORMAL}
Show all processes accessing a specific directory-->
-A user must be introduced after the -u option-
-The user must be a real user on your computer-  
_EOF_
}

show_error() {
  echo -e "${RED}ERROR: $1${NC}"
  echo -e "${RED}Please, read carefully the manual on how to use the program${NC}"
  echo $LINE
  show_help
  echo $LINE
  exit 1
}

# Process all options introduced by the user
while [ "$1" != "" ]; do
  case $1 in
    -u )
      shift
      if [ -z "$1" ]; then
        show_error "Option -u requires an argument"
      fi
      USERS=$1
      if ! grep -q "^$USERS:" /etc/passwd; then
        show_error "User '$USERS' not found"
        exit 1
      fi
      ;;
    -d )
      shift
      if [ -z "$1" ]; then
        show_error "Option -d requires an argument"
        exit 1
      fi
      ROUTE=$1
      if [ ! -d "$ROUTE" ]; then
        show_error "Directory '$ROUTE' does not exist"
        exit 1
      fi
      ;;
    -z )
      SIDZERO=true
      ;;
    -h )
      show_help
      exit 0
      ;;
    * )
      show_error "Unknown parameter: $1"
      exit 1
      ;;
  esac
  shift
done

# Execute the ps command
get_ps() {
  ps ax -o sid,pgid,pid,user,tty,%mem,cmd --sort=user
}

apply_filters() {
  ps_output=$(get_ps)
  
  if [ -n "$USERS" ]; then
    ps_output=$(echo "$ps_output" | awk -v user="$USERS" '$4 == user')
  fi
  
  if [ "$SIDZERO" = false ]; then
    ps_output=$(echo "$ps_output" | awk '$1 != 0')
  fi
  
  if [ -n "$ROUTE" ]; then
    pids=$(lsof +d "$ROUTE" | awk 'NR>1 {print $2}' | uniq | tr '\n' '|')
    if [ -z "$pids" ]; then
      echo -e "${CYAN}No process is currently running the specified directory${NC}"
      exit 0
    fi
    pids_array="${pids%|}"
    ps_output=$(echo "$ps_output" | grep -E "$pids_array")
    if [ -z "$ps_output" ]; then
      echo -e "${CYAN}No process is currently running the specified directory${NC}"
      exit 0
    fi
  fi
  # COUNTER=0
  # echo "$ps_output" | awk -v var="$COUNTER$" '$1 == 0 { var++ } END { print var }'
  echo "$ps_output"
}

check_commands
apply_filters

exit 0