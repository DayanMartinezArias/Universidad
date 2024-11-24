#!/bin/bash

iverilog microc_tb.v componentes.v alu.v memprog.v microc.v 
vvp ./a.out
gtkwave microc_tb.gtkw &
exit 0