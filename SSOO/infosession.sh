#!/bin/bash

PROGNAME=$(basename $0)
USRS=""

show_process() {
  if [ "$USRS" == "" ]; then
    ps ax -o sid,pgid,pid,user,tty,%mem,cmd --sort=user | awk '$1 != 0'
  else
    ps ax -o sid,pgid,pid,user,tty,%mem,cmd -u $USRS --sort=user | awk '$1 != 0'
  fi
}

show_process_sid_zero() {
  if [ "$USRS" == "" ]; then
    ps ax -o sid,pgid,pid,user,tty,%mem,cmd --sort=user
  else
    ps ax -o sid,pgid,pid,user,tty,%mem,cmd -u$USRS --sort=user 
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
  echo ERROR: An Unknown param was introduced: $1
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
        if [[ "$2" == "" ]]; then
          echo "Opción $1 requiere un argumento"
          exit 1
        fi
        USRS=$2
        shift
        ;;
      -z )
        show_process_sid_zero
        exit 0
        ;;
      * )
        unknown_param $1
        exit 1
      esac
      shift
  done
}

check_help "$@"
read_args "$@" 

show_process 
exit 0

## COSAS A HECER ##

# ERROR PARA COMANDOS DOBLES
# ASEGURASE DE QUE EXISTE EL NOMBRE DE USUARIO
# MEJORAR LA FORMA MOTRAR LOS PROCESOS POR USUARIO

