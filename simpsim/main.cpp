////////////////////////////////////////////////////////////////////////////////
// PID controller simulator, demonstrates use of libpidsim.
//

#include <stdio.h>
#include <stdlib.h> // for rand()
#include <unistd.h> // for getopt

#include "pidsim.h"

////////////////////////////////////////////////////////////////////////////////

// simple pland - a first order low pass with noisy output system
class Plant : public PidSim::IPlant {
public:
	Plant() : 
		_prevTime(0), _prevState(0) 
	{}
	
	// computes feedback signal from plant
	double compute(double input, double time) {
		double ts = time - _prevTime;
		double cutoffFreq = 5; // Hz
		double tau = 1 / ( 2 * PI * cutoffFreq ); // time constant tRC
		double alpha = tau / (tau + ts);
		double out = alpha * _prevState + (1 - alpha) * input;
		double noise = 0.1 * (double(rand()) / double(RAND_MAX) - 0.5);
		out += noise;
		_prevState = out;
		_prevTime = time;
		return out;
	}
	
private:
	double _prevTime;
	double _prevState;	
};

////////////////////////////////////////////////////////////////////////////////
//
// global parameters
// defaults adjusted manually to get a resonable step response from the plant above
//
double kp = 0.2;
double ki = 10;
double kd = 0.1;
double lpf = 5; // 5Hz
double tsample = 1.0/100.0; // default 100Hz
double tend = 5;

////////////////////////////////////////////////////////////////////////////////

void parseArgs(int argc, char **argv) {
	bool ok = true;
	char c;
	while ((c = getopt (argc, argv, "p:i:d:s:e:f:")) != -1) {
		switch (c) {
			case 'p' : kp = atof(optarg); break;
			case 'i' : ki = atof(optarg); break;
			case 'd' : kd = atof(optarg); break;
			case 'f' : lpf = atof(optarg); break;
			case 's' : tsample = atof(optarg); break;
			case 'e' : tend = atof(optarg); break;
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
			);
	}
}

////////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv) {

	parseArgs(argc, argv);
	
	PidSim sim;
	sim.setPlant(new Plant());
	
	sim.setKp(kp);
	sim.setKd(kd);
	sim.setLPF(lpf);
	sim.setKi(ki);
	
	// simulation loop and output of csv values
	
	double curTime = 0;
	while( curTime < tend ) {
		sim.step(curTime);
		printf( "%f, %f, %f, %f, %f\n", curTime, 
				sim.getStimulus(), 
				sim.getError(), 
				sim.getControl(), 
				sim.getFeedback() );
		curTime += tsample;
	}
}

