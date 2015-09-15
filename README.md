# pidsim
a discrete-time simulation library experiments

Overview
----------

This set of code provides an environment for simulating a discrete time control systems.
The system can be build by creating a set of 'processors'.
Each processor generates output which can be linked to another processors.
The simulation is executed by Simulation class on a given set of processors.
The simulation in each step updates state of processors in registration order. 
The simulation also outputs to console output values of requested processors. 
The csv format contains "time, stimulus, error, control, plant" values.

One can plot it with :
gnuplot "plot "data.csv" using 1:2 with lines, "data.csv" using 1:5 with lines"

Also:
- a simple simulation of pid is provides in 'simpsim' subdirectory.
- a simple FIR filter and test program is in 'fir' subdirectory.

TODO
-----
code cleanup: 
  * separate classes into individual headers
  * move more complicated funcion definitions to .cpp files
  * error handing (missed links would crash execution with NULL pointer)
  * separate output to stdout from simulation
  * optimization
 

BUILD
------

Buiild is using cmake/make system now. There is a 'build' script provided.

To build just type:
./build

The output of build is now in 'out' directory. 
The executable is 'out/pidsimtest'

There is also a 'cleanup' script provided.

Also, a simple, primitive makefile is provided.
