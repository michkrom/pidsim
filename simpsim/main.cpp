////////////////////////////////////////////////////////////////////////////////
//
// PID controller simulator, demonstrates use of libpidsim.
//

#include <stdio.h>
#include <stdlib.h> // for rand()
#include <unistd.h> // for getopt

#include "pidsim.h"
#include "stimulus.h"

////////////////////////////////////////////////////////////////////////////////

// simple plant - a first order low pass with noisy output system
class Plant : public PidSim::IProcessor {
public:
	Plant(double cutoffFreq, double noiseAmpl) : 
		_cutoffFreq(cutoffFreq), _noiseAmpl(noiseAmpl),
		_prevTime(0), _prevState(0)
	{}
	
	// computes feedback signal from plant
	double compute(double input, double time) {
		double ts = time - _prevTime;
		double tau = 1 / ( 2 * PI * _cutoffFreq ); // time constant tRC
		double alpha = tau / (tau + ts);
		double out = alpha * _prevState + (1 - alpha) * input;
		double noise = _noiseAmpl * (double(rand()) / double(RAND_MAX) - 0.5);
		out += noise;
		_prevState = out;
		_prevTime = time;
		return out;
	}
	
private:
	double _cutoffFreq;
	double _noiseAmpl;
	double _prevTime;
	double _prevState;	
};

////////////////////////////////////////////////////////////////////////////////

//
// global parameters
// defaults adjusted manually to get a resonable step response from the plant above
//
// PID params
double kp = 1;
double ki = 10;
double kd = 0.2;
double lpf = 50; // 50Hz
// simulation params
double tSample = 1.0/100.0; // default 100Hz
double tEnd = 5; // seconds
char stimulusType = 's';
// plant's params
double noiseAmpl = 0.1;
double cutoffFreq = 1; // Hz

////////////////////////////////////////////////////////////////////////////////

void parseArgs(int argc, char **argv) {
	bool ok = true;
	char c;
	while ((c = getopt (argc, argv, "p:i:d:s:e:f:x:n:c:")) != -1) {
		switch (c) {
			case 'p' : kp = atof(optarg); break;
			case 'i' : ki = atof(optarg); break;
			case 'd' : kd = atof(optarg); break;
			case 'f' : lpf = atof(optarg); break;
			case 's' : tSample = atof(optarg); break;
			case 'e' : tEnd = atof(optarg); break;
			case 'x' : stimulusType = optarg[0]; break;
			case 'n' : noiseAmpl = atof(optarg); break;
			case 'c' : cutoffFreq = atof(optarg); break;
			case '?' : 
				ok = false;
				fprintf(stderr, "option %c invalid \n", optopt); 
				break;
		}
	}
	if(!ok) {
		fprintf(stderr, 
			"pidsim -pV -iV -dV -fV -sV -eV\n"
			"p,i,d - PID gains\n"
			"f - cut off frequency of D branch filter\n"
			"s - sample period\n"
			"e - end simulation time\n"
			"x - stimulus type: impulse, step, ramp\n"
			"n - plant's noise level\n"
			"c - plant's cutoff Frequency\n"
		);
		exit(1);
	}
}

////////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv) {

	parseArgs(argc, argv);
	
	// set up simulation
	
	PidSim sim;
	
	// set up the plant
	Plant plant(cutoffFreq, noiseAmpl);
	sim.setPlant(&plant);
	
	// select stimulus
	PidSim::IProcessor* pStim = 0;
	switch(stimulusType) {
		case 'r' : pStim = new Ramp(1,1); break;
		case 'i' : pStim = new Impulse(1); break;		
		case 's' : 
		default:   pStim = new Step(1); break;
	}	
	sim.setStimulus(pStim);
	
	// set PID parameters
	sim.setKp(kp);
	sim.setKd(kd);
	sim.setLPF(lpf);
	sim.setKi(ki);
	
	// simulation loop and output of csv values
	
	double curTime = 0;
	while( curTime < tEnd ) {
		sim.step(curTime);
		printf( "%f, %f, %f, %f, %f\n", curTime, 
				sim.getStimulus(), 
				sim.getError(), 
				sim.getControl(), 
				sim.getFeedback() );
		curTime += tSample;
	}
	
	// not all objects are properly destructed at this point
	// but since we exit the process it's not a big deal
	// ideally the IProcessor would have a virtual destructor and either 
	// an explicit delete would suffice here or the Sim could own and clean up.
}

