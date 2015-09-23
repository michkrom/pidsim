# pidsim

A discrete-time simulation experiments

Overview
----------

This set of code provides an environment for simulating a discrete time control systems.

There are three projects here:
- simpsim - a simple PID controller simulator with ability to specify the plant, the stimulus and PIC parameters.
- sim - a flexible but somewhat more complicated simulator where system can be defined dynamically out of generic processor such as gain, sum, integrator, differentiator etc.
- fir - test for building and simulating FIR filters

TODO
-----
- combine projects in such a way the components can be shared/interchanged 

BUILD
------

Each projects has separate build system.
Build is using cmake/make system now. There is a 'build' script provided.
Requires standard make/gcc toolchain (build-essentials on ubuntu/debian) and cmake.

To build just type e.g. :
cd simpsim
./build

The output of build is in 'out' directory. 
The executable is 'out/pidsimtest'

There is also a 'cleanup' script provided.

Note: 'fir' still uses makefile so just 'make' would suffice.
