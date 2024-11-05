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
stop=0
e_opt=0
t_opt=0
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
  echo -e "${RED}Please, read carefully the manual on how to use the program${NC}" 1>&2
  echo $LINE 1>&2
  show_help 1>&2
  echo $LINE 1>&2
  exit 1
}

count=0

read_users() {
  for usr in "$@"; do
  if [[ $usr =~ -.* ]]; then 
    break
  fi
  users="$users"$usr" "
  count=$(($count + 1))
  done
}

# Process all options introduced by the user
while [ "$1" != "" ]; do
  case $1 in
    -u )
      shift
      if [ "$1" == "" ]; then
        show_error "Option -u requires an argument"
      fi
      read_users "$@"
      for arg in "$@"; do
        if [ $count -ne 1 ]; then
          shift
          count=$(($count - 1))
        fi
      done
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
    -e )
      e_opt=1
      ;;
    -t )
      t_opt=1
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
  # To check if the user exists
  for check in $1 ; do
    if ! grep -q "$check" /etc/passwd; then
      show_error "User '$check' not found"
      exit 1
    fi
  done
  
  if [ "$1" != "" ]; then
    filtered_processes=""
    for user in $1; do
      filtered_processes+="$(echo "$2" | awk -v usr="$user" '$4 == usr')\n"
    done
    echo "$filtered_processes"
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
    pids=$(lsof +d "$1" | awk 'NR>1 {print $2}' | uniq | tr '\n' ' ') # To transform the pids into a regex
    if [ "$pids" == "" ]; then
      echo -e "${CYAN}No process is currently running the specified directory${NC}"
      exit 0
    fi

    filtered_pids=""
    for pid in $pids; do
      filtered_pids+="$(echo "$2" | awk -v pid="$pid" '$2 == pid')\n"
    done

    new_ps=$(echo -e "$filtered_pids" | grep -v "^$")

    if [ "$new_ps" == "" ]; then
      echo -e "${CYAN}No process is currently running in the specified directory${NC}"
      exit 0
    fi

    echo "$new_ps"
  else 
    echo "$2"
  fi

}

check_terminal() {
  if [ $t_opt -eq 1 ]; then 
    echo "$1" | awk -v symbol="?" '$5 != symbol'
  else 
    echo "$1"
  fi
}

session_table() { 
  ps_output="$1" 
  sessions=$(echo "$ps_output" | awk 'NR>1 {print $1}' | uniq)

  for sid in $sessions; do
    process=$(echo "$ps_output" | awk -v sid="$sid" '$1 == sid')

    num_groups=$(echo "$process" | awk '{print $2}' | uniq | wc -l)
    leader_pid=$(echo "$process" | awk -v sid="$sid" '$1 == sid && $3 == sid {print $3}') 
    leader_user=$(echo "$process" | awk -v pid="$leader_pid" '$3 == pid {print $4}') 
    leader_tty=$(echo "$process" | awk -v pid="$leader_pid" '$3 == pid {print $5}') 
    leader_cmd=$(echo "$process" | awk -v pid="$leader_pid" '$3 == pid {print $7}')

    if [ "$leader_pid" == "" ]; then 
      leader_pid="?" 
      leader_user="?" 
      leader_tty="?" 
      leader_cmd="?"
    fi

    final_res+="$sid\t$num_groups\t$leader_pid\t$leader_user\t$leader_tty\t$leader_cmd\n" 
  done
  echo -e $final_res | sort --key 4
}

filter() {

  ps_output=$(get_ps)
  ps_output=$(check_users "$users" "$ps_output") 
  ps_output=$(check_sid "$sidzero" "$ps_output")
  ps_output=$(check_route "$route" "$ps_output")
  ps_output=$(check_terminal "$ps_output")
  
  if [ $e_opt -eq 1 ]; then 
    echo "$ps_output" 
  else 
    session_table "$ps_output" 
  fi
}

check_commands
filter

exit 0