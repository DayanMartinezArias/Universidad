#!/bin/bash

# Universidad de la Laguna
# Sistemas Operativos
# Dayán Martínez Arias 
# alu0101644561
# PE102
# PRIMERA ENTREGA: 30/10/24

#!/bin/bash

# Global variables
PROGNAME=$(basename $0)
USERS=""
ROUTE=""
PIDZERO=false

# Color definitions
RED='\033[0;31m'
NC='\033[0m'

# Check for requiered commands
if ! command -v ps >/dev/null 2>&1 || ! command -v awk >/dev/null 2>&1 || ! command -v lsof >/dev/null 2>&1; then
  echo -e "${RED}ERROR: This script requires awk, lsof, and ps installed on your computer${NC}"
  exit 1
fi

# Function to show a help message
show_help() {
  cat << _EOF_
Usage: $PROGNAME [OPTIONS]

Options:
  -h             Show this help message and exit
  -z             Show all processes with SID 0
  -u user        Show all processes of a specific user
  -d directory   Show all processes accessing a specific directory
_EOF_
}

# Process all options introduced by the user
while [ "$1" != "" ]; do
  case $1 in
    -u )
      shift
      if [ -z "$1" ]; then
        echo -e "${RED}ERROR: Option -u requires an argument${NC}"
        exit 1
      fi
      USERS=$1
      if ! grep -q "^$USERS:" /etc/passwd; then
        echo -e "${RED}ERROR: User '$USERS' not found${NC}"
        exit 1
      fi
      ;;
    -d )
      shift
      if [ -z "$1" ]; then
        echo -e "${RED}ERROR: Option -d requires an argument${NC}"
        exit 1
      fi
      ROUTE=$1
      if [ ! -d "$ROUTE" ]; then
        echo -e "${RED}ERROR: Directory '$ROUTE' does not exist${NC}"
        exit 1
      fi
      ;;
    -z )
      PIDZERO=true
      ;;
    -h )
      show_help
      exit 0
      ;;
    * )
      echo -e "${RED}ERROR: Unknown parameter: $1${NC}"
      show_help
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
  
  if [ "$PIDZERO" = false ]; then
    ps_output=$(echo "$ps_output" | awk '$1 != 0')
  fi
  
  if [ -n "$ROUTE" ]; then
    pids=$(lsof +D "$ROUTE" 2>/dev/null | awk 'NR>1 {print $2}' | sort -u | tr '\n' ',')
    ps_output=$(echo "$ps_output" | awk -v pids="$pids" '
      BEGIN { 
        split(pids, pid_array, ",")
      }
      { for (i in pid_array) if ($3 == pid_array[i]) print }
    ')
  fi

  echo "$ps_output"
}

apply_filters