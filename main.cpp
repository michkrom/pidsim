#include <stdio.h>

#include "stimulus.h"
#include "plants.h"
#include "processors.h"
#include "filters.h"
#include "PIDProcessor.h"

#include "simulation.h"


#define PI           3.14159265358979323846  /* pi */


// very simple system showing an averaging plant stimulated with an impulse
void test1()
{
	IProcessor* stim = new ImpulseStimulus(1);
	SingleInputProcessor* plant = new AveragingPlant();
	
	// link plant input with stimulus output
	plant->setInput( stim );

	Simulation sim;
	sim.add(stim, true);
	sim.add(plant, true);
	
	// run for 2 seconds with 0.1 sampling period (10Hz)
	sim.run(2, 0.1);
}


// full pid controlled system with a step stimulus (@ 02.second)
void test2()
{
	IProcessor* stim = new StepStimulus(0.2);
	ErrorProcessor* error = new ErrorProcessor();
	PIDProcessor* controller = new PIDProcessor();
	SingleInputProcessor* plant = new AveragingPlant();
	controller->setKP(1);
	// need integration to combat a delay of AveragingPlant
	controller->setKI(2);
	//controller->setKD(0.1);
	
	// link the system
	error->setTargetInput( stim );
	error->setFeedbackInput( plant );
	plant->setInput( controller );
	controller->setInput( error );

	Simulation sim;
	sim.add(stim, true);
	sim.add(error, true);
	sim.add(controller, true);
	sim.add(plant, true);
	
	// run for 10 seconds with 0.1 sampling period (10Hz)
	sim.run(10, 0.1);	
}



// PID-controlled plant that has an RC-line first order behaviour
void test3()
{
	double samplePeriod = 0.05;
	//IProcessor* stim = new ImpulseStimulus(0.2);
	IProcessor* stim = new StepStimulus(0.2);
	ErrorProcessor* error = new ErrorProcessor();
	PIDProcessor* controller = new PIDProcessor();
	// plant is an first order filter
	double cutoffFreq = 0.3; 
	double timeConst = 1 / ( 2 * PI * cutoffFreq );
	SingleInputProcessor* plant = new FirstOrderLowPass(timeConst);
	controller->setKP(1);
	// need differentiation to combat RC response
	//controller->setKD(0.3);
	// need integration to combat delay
	controller->setKI(0.3);
	
	// link the system
	error->setTargetInput( stim );
	error->setFeedbackInput( plant );
	//plant->setInput( controller );
	plant->setInput( stim );
	controller->setInput( error );

	Simulation sim;
	sim.add(stim, true);
	sim.add(error, true);
	sim.add(controller, true);
	sim.add(plant, true);
	
	sim.run(10, samplePeriod);	
}


// build up a PID controlled system from basic parts
// and inject a first order low pass filter in D path
// "plant" is also a first order low pass with noisy output
// hence the system is of second order
void test4()
{
	double samplePeriod = 0.01;
	//IProcessor* stim = new ImpulseStimulus(0.2);
	IProcessor* stim = new StepStimulus(1);
	
	ErrorProcessor* error = new ErrorProcessor();
	error->setTargetInput(stim);	
	
	Gain* KP = new Gain(1);
	KP->setInput(error);
	Differentiator* df = new Differentiator();
	df->setInput(error);
	Gain* KD = new Gain(1);	
	KD->setInput(df);
	
	double cutoffFreq = 10; // [Hz]
	double timeConst = 1 / ( 2 * PI * cutoffFreq );
	FirstOrderLowPass* dfilter = new FirstOrderLowPass(timeConst);
	dfilter->setInput(KD);
	
	Integrator* in = new Integrator();
	in->setInput(error);
	Gain* KI = new Gain(0.5);
	KI->setInput(in);
	
	Summer* sum = new Summer();
	sum->addInput(KP);
	sum->addInput(KI);
	sum->addInput(dfilter);
	
	// plant
	cutoffFreq = 0.5; // [Hz]
	timeConst = 1 / ( 2 * PI * cutoffFreq );
	FirstOrderLowPass* plant = new FirstOrderLowPass(timeConst);
	plant->setInput( sum );
	NoiseAdder* noise = new NoiseAdder(0.3);
	noise->setInput( plant );
	error->setFeedbackInput( noise );
		
	Simulation sim;
	sim.add(stim, true);
	sim.add(error, true);
	sim.add(df, false);
	sim.add(in, false);
	sim.add(KP, false);
	sim.add(KI, false);
	sim.add(KD, false);
	sim.add(dfilter, false);
	sim.add(sum, true);
	sim.add(plant, false);
	sim.add(noise, true);
	
	sim.run(20, samplePeriod);	
}

////////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv) 
{
	//test1();
	//test2();
	//test3();
	test4();
}

