#ifndef _SIMULATION_INCLUDED
#define _SIMULATION_INCLUDED


#include <stdio.h>

#include "IProcessor.h"

#include <list>
#include <set>

// Simulaton 
//
// it has many processors that make up a system
// it can run simulation of a system by stepping through time
// at each sample time it will ask each processor to update itself
// the order of updating is the order in which processors were added
// it makes no attemp to unroll loops (feedbacks), 
// hence the order is of importance
//
class Simulation
{
public:
	Simulation() {}

	void add(IProcessor* p, bool printOutput = false) { 
		_processors.push_back(p);
		if( printOutput )
			_outputs.insert(p);
	}

	void run(double endTime, double sampleTime);
	
	void runStep(double time);
	
private:
	typedef std::list<IProcessor*> Processors;
	Processors _processors;
	std::set<ISource*> _outputs;
};

#endif
