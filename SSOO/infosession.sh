#!/bin/bash

# Dayán Martínez Arias 
# alu0101644561
# PE102

# Style definitions
RED='\033[0;31m'
YELLOW='\033[1;33m'
CYAN='\033[0;36m'
NC='\033[0m'
BOLD=$(tput bold)
NORMAL=$(tput sgr0)

# Variables
PROGNAME=$(basename $0)
users=""
route=""
sidzero=0
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
  echo -e "${RED}ERROR: $1${NC}" 1>&2
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
      if [ "$1" == "" ]; then
        show_error "Option -u requires an argument"
      fi
      users=$1
      ;;
    -d )
      shift
      if [ "$1" == "" ]; then
        show_error "Option -d requires an argument"
        exit 1
      fi
      route=$1
      if [ ! -d "$route" ]; then
        show_error "Directory '$route' does not exist"
        exit 1
      fi
      ;;
    -z )
      sidzero=1
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

check_users() {
  # To filter by users
  if [ "$1" != "" ]; then
    echo "$2" | awk -v user="$users" '$4 == user'
  # To check if the user exists
  if ! grep -q "^$1:" /etc/passwd; then
    show_error "User '$users' not found"
    exit 1
  fi
  else
    echo "$2" 
  fi
}

check_sid() {
  if [ $1 -eq 0 ]; then
    echo "$2" | awk '$1 != 0'
  else
    echo "$2"
  fi
}

check_route() {
  if [ "$1" != "" ]; then
    pids=$(lsof +d "$1" | awk 'NR>1 {print $2}' | uniq | tr '\n' '|') # To transform the pids into a regex
    if [ "$pids" == "" ]; then
      echo -e "${CYAN}No process is currently running the specified directory${NC}"
      exit 0
    fi
    reg_expr="${pids%|}" # To eliminate the last |
    new_ps=$(echo "$2" | grep -E "$reg_expr") # grep -E is used to match the patterns
    if [ "$new_ps" == "" ]; then
      echo -e "${CYAN}No process is currently running the specified directory${NC}"
      exit 0
    fi
    echo "$new_ps"
  else 
    echo "$2"
  fi
}

filter() {
  ps_output=$(get_ps)
  ps_output=$(check_users "$users" "$ps_output") 
  ps_output=$(check_sid "$sidzero" "$ps_output")
  ps_output=$(check_route "$route" "$ps_output")
  echo "$ps_output"
}

check_commands
filter

exit 0