#!/bin/bash

NAME=".bashrc"

if [ -f $NAME ]; then
  echo "Tienes un fichero llamado $NAME"
else 
  echo "No tienes un fichero llamado $NAME"
fi