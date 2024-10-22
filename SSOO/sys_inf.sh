#!/bin/bash

# Sistemas Operativos

### CONSTANTES
TITLE="Información del sistema para $HOSTNAME"
RIGHT_NOW=$(date +"%x %r %Z")
TIME_STAMP="Actualizado el $RIGHT_NOW por $USER"

### ESTILOS
TXT_BOLD=$(tput bold)
TXT_GREEN=$(tput setaf 2)
TXT_RESET=$(tput sgr0)
TXT_UNDERLINE=$(tput smul)

### FUNCIONES

system_info() {
  echo "{$TEXT_UNLINE}Versión del sistema$TXT_RESET"
  uname -a
}

show_uptime() {
  echo
  echo "${TEXT_UNLINE}Tiempo de encendido del sistema$TXT_RESET"
  uptime
}

drive_space() {
  echo ""
}

home_space() {
  if [ "$USER" != root ]; then
    du -sh /home/"$USER"/ | tr "/" " " | awk '{print $1, $4}'
  else 
    du -h /home
  fi
}

cat << _EOF_
$TXT_BOLD$TITLE$TXT_RESET

$(system_info)
$(show_uptime)
$(drive_space)
$(home_space)
$TXT_GREEN$TIME_STAMP$TXT_RESET
_EOF_