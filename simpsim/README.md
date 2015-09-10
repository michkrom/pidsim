# simple pidsim
A discrete-time PID control system simulation library and example.

Overview
----------

This is a PID constroller system simulation example.
The example code in main.cpp creates a simple plant 
and the runs simulation and output state to console in csv format.

usage:
pidsim -pV -iV -dV -fV -sV -eV
	p,i,d - PID gains
	f - cut off frequency of D branch filter
	s - sample period
	e - end simulation time
pidsim >out.csv
gnuplot -e "plot "out.csv" using 1:2 with lines, "out.csv" using 1:5 with lines" --persists

BUILD
------
A simple, primitive makefile is provided so just make it!

There is libpidsim in "lib" subdirectory and aldo has a makefile but the 
main make file would automatically execute the library build.

TODO
-----
abstract stimulus so user can also provide it
