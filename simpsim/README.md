# simple pidsim
A discrete-time PID control system simulation library and example.

Overview
----------
This is a PID constroller system simulation example.
The example code in main.cpp creates a simple plant 
and the runs simulation and output state to console in csv format.

USAGE
------
pidsim -pV -iV -dV -fV -sV -eV
	p,i,d - PID gains
	f - cut off frequency of D branch filter
	s - sample period
	e - end simulation time
	
> pidsim  -p 1 -f 50 -d 0.2 -i 10 -n 0.05 -s 0.005 >out.csv
> gnuplot -e "plot "out.csv" using 1:2 with lines, "out.csv" using 1:5 with lines" --persists


BUILD
------
A simple, primitive makefile is provided so just make it!

There is libpidsim in "lib" subdirectory and aldo has a makefile but the 
main make file would automatically execute the library build.

TODO
-----
abstract stimulus so user can also provide it
