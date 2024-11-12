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
BASHUSER=$(whoami)
users=""
route=""
sidzero=0
stop=0
e_opt=0
t_opt=0
sm_opt=0
sg_opt=0
reverse=0
count=0
LINE="==========================================================="
WARNING=$(echo -e "${YELLOW}It is important to have awk, lsof, grep and ps previously installed!!${NC}")
NOMATCH=$(echo -e "${CYAN}SEEMS LIKE THERE IS NOT ANY MATCHES FOR THE SPECIFIED OPTIONS${NC}")
DEF=$(echo -e "${YELLOW}By default the table is sorted by username!!!${NC}")
ARROW=$(echo -e "${YELLOW}->${NC}")

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
$LINE
                   SYSTEM INFORMATION
                      HELP MANUAL
$LINE
Usage: $PROGNAME [OPTIONS]

$WARNING

${BOLD}-h:${NORMAL}
Show this help message and exit

${BOLD}-z:${NORMAL}
Show all processes with SID 0 

${BOLD}-u user:${NORMAL}
Show all processes of a specific user
${ARROW}A user must be introduced after the -u option
${ARROW}The user must be a real user on your computer

${BOLD}-d directory:${NORMAL}
Show all processes accessing a specific directory
${ARROW}A user must be introduced after the -u option
${ARROW}The user must be a real user on your computer

${BOLD}-e:${NORMAL}
Process table mode, where information about running processes is shown.
${ARROW}When this option isn't selected, the the program shows information about sessions

${BOLD}-t:${NORMAL}
Shows only processes wich have an associated terminal

${BOLD}Sorting options:${NORMAL}
-sm: Sorts by memory usage
-sg Sorts by numbers of groups of a session
-r Reverts all sorting methods
$DEF
_EOF_
}

show_error() {
  echo -e "${RED}${BOLD}$LINE${NC}"
  echo -e "${RED}${BOLD}                   AN ERROR HAPPENED          ${NC}"
  echo -e "${RED}${BOLD}$LINE${NC}"
  echo -e "${RED}${BOLD}ERROR MESSAGE: ${NC}${RED}$1${NC}" 1>&2
  echo
  echo -e "${YELLOW}Please, read carefully the manual below on how to use the program${NC}" 1>&2
  show_help 1>&2
  exit 1
}

read_users() {
  for usr in "$@"; do
  if [[ $usr =~ -.* ]]; then 
    break
  fi
  users="$users"$usr" "
  count=$(($count + 1))
  done
}

block_rep() {
  if [ "$1" != "" ]; then
    show_error "Duplicated option, please avoid doing this with -d and -u"
  fi
}


# Process all options introduced by the user
while [ "$1" != "" ]; do
  case $1 in
    -u )
      shift
      # Other commands options don't really affect that much the program if the're repeated. 
      # But -u repeated could lead to unexpected behaviour
      block_rep "$users"
      if [ "$1" == "" ]; then
        show_error "Option -u requires an argument"
      fi
      read_users "$@"
      # After read_users, all users will be stored in a variable
      # Also, there will be a counter with x value of users
      # So the program can shift them all to the next option
      for arg in "$@"; do
        if [ $count -ne 1 ]; then
          shift
          count=$(($count - 1))
        fi
      done
      ;;
    -d )
      shift
      # Same thing as -u happens with option -d
      block_rep "$route"
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
    -sm )
      sm_opt=1
      ;;
    -sg )
      sg_opt=1
      ;;
    -r )
      reverse=1
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
    filt_proc=""
    for user in $1; do
      filt_proc+="$(echo "$2" | awk -v usr="$user" '$4 == usr')"
      filt_proc+=$'\n'
    done
    echo "$filt_proc"
  else
    echo "$2" | awk -v usr="$BASHUSER" '$4 == usr'
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
    pids=$(lsof +d "$1" | awk 'NR>1 {print $2}' | uniq | tr '\n' ' ') 
    if [ "$pids" == "" ]; then
      echo -e "${CYAN}No process is currently running the specified directory${NC}"
      exit 0
    fi

    filtered_pids=""
    for pid in $pids; do
      filtered_pids+="$(echo "$2" | awk -v pid="$pid" '$2 == pid')"
      filtered_pids+=$'\n' 
    done

    if [ "$filtered_pids" == "" ]; then
      echo -e "${CYAN}No process is currently running in the specified directory${NC}"
      exit 0
    fi

    echo "$filtered_pids" | grep -v '^$'
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
  ps_tab="$1" 
  sessions=$(echo "$ps_tab" | awk 'NR>1 {print $1}' | uniq)

  for sid in $sessions; do
    process=$(echo "$ps_tab" | awk -v sid="$sid" '$1 == sid')

    num_groups=$(echo "$process" | awk '{print $2}' | uniq | wc -l)
    leader_pid=$(echo "$process" | awk -v sid="$sid" '$1 == sid && $3 == sid {print $3}') 
    total_mem=$(echo "$process" | LC_ALL=C awk '{mem_sum += $6} END {print mem_sum}')
    leader_user=$(echo "$process" | awk -v pid="$leader_pid" '$3 == pid {print $4}') 
    leader_tty=$(echo "$process" | awk -v pid="$leader_pid" '$3 == pid {print $5}') 
    leader_cmd=$(echo "$process" | awk -v pid="$leader_pid" '$3 == pid {print $7}')

    if [ "$leader_pid" == "" ]; then 
      leader_pid="?" 
      leader_user="?" 
      leader_tty="?" 
      leader_cmd="?"
    fi
    final_res+="$sid\t$num_groups\t$leader_pid\t$leader_user\t$leader_tty\t$total_mem\t$leader_cmd\n" 
  done
  echo -e $final_res | sort --key 4 | uniq
}

order_by() {
  ordered=""
  rev=""
  if [ $sm_opt -eq 1 ]; then
    ordered=$(echo "$1" | sort -V -k 6)
    rev=$(echo "$1" | sort -r -V -k 6)
  elif [ $sg_opt -eq 1 ]; then
    ordered=$(echo "$1" | sort -k 2)
    rev=$(echo "$1" | sort -r -k 2)
  else 
    ordered=$(echo "$1" | sort -k 4)
    rev=$(echo "$1" | sort -r -k 4)
  fi

  if [ $reverse -eq 1 ];then
    echo "$rev"
  else 
    echo "$ordered"
  fi
}

filter() {

  ps_tab=$(get_ps)
  ps_tab=$(check_users "$users" "$ps_tab") 
  ps_tab=$(check_sid "$sidzero" "$ps_tab")
  ps_tab=$(check_route "$route" "$ps_tab")
  ps_tab=$(check_terminal "$ps_tab")

  if [ "$ps_tab" == "" ];then 
    echo -e $NOMATCH
    exit 0
  fi

  final_table=""

  if [[ ($sg_opt -eq 1 && $sm_opt -eq 1) || ($sg_opt -eq 1 && $e_opt -eq 1) ]]; then
    show_error "Invalid options : -sg isn't compatible with -sm and -e"
  fi

  if [ $e_opt -eq 1 ]; then 
    # sid,pgid,pid,user,tty,%mem,cmd
    printf "%-10s %-10s %-10s %-10s %-10s %-10s %s\n" "SID" "PGID" "PID" "USR" "TTY" "MEM" "CMD"
    final_table="$ps_tab"
  else 
    printf "%-10s %-10s %-10s %-10s %-10s %-10s %s\n" "SID" "N_groups" "PID" "USR" "TTY" "MEM" "CMD"
    final_session_table=$(session_table "$ps_tab")
    final_table="$final_session_table" 
  fi
  
  final_table=$(order_by "$final_table")
  echo "$final_table"
}

check_commands
filter

exit 0