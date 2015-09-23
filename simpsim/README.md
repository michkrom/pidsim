# simple pidsim
A discrete-time PID control system simulation library and example.

Overview
----------
This is a PID constroller system simulation example.
The code uses a 'lib' that implements PID and interface to set 
plant and stimulus.

The example code in main.cpp creates a simple plant and a stimulus.
It then runs the simulation and outputs state to stdout in CSV format: 
"time,stimulus,error,control,feedback".

The simulation loop is implemented in main.cpp and user has control on progress of time, ie. samping rate. 
This allow simulation of system with jittery clocks (uneven sampling).
The specification of cut-off frequency is in SI units [Hz] while the time is in [s].
To achive system update (sample rate) of say 10Hz, the time must be stepped by 0.1s = 1/10Hz.

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
A cmake build system is in place. A build script for out-or-source build is provided.

The libpidsim i sin "lib" subdirectory and has it's own build script, however the main test program is automaticly invoking it.

TODO
-----
- cleaner desctruction or plant and stimulus
