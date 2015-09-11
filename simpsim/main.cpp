////////////////////////////////////////////////////////////////////////////////
// PID controller simulator, demonstrates use of libpidsim.
//

#include <stdio.h>
#include <stdlib.h> // for rand()
#include <unistd.h> // for getopt

#include "pidsim.h"

////////////////////////////////////////////////////////////////////////////////

// simple plant- a first order low pass with noisy output system
class Plant : public PidSim::IProcessor {
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

// step function
class Step : public PidSim::IProcessor {
public:
	Step(double time) : _time(time) {}
	double compute(double input, double time) {
		return time > _time ? 1 : 0;
	}
protected:
	double _time;
};

////////////////////////////////////////////////////////////////////////////////

// impulse function
class Impulse : public PidSim::IProcessor {
public:
	Impulse(double time) : _time(time), _fired(false) {}
	double compute(double input, double time) {
		bool pulse = time > _time && !_fired;
		_fired = _fired || pulse;
		return pulse ? 1 : 0;
	}
protected:
	double _time;
	bool _fired;
};

// ramp function starts at given time and goes 0..1 with given slope 
class Ramp : public PidSim::IProcessor {
public:
	Ramp(double time, double slope) : _time(time), _slope(slope) {}
	double compute(double input, double time) {
		double deltaT = time - _time;
		double out = deltaT > 0 ? _slope * deltaT : 0;
		out = out > 1 ? 1 : out;
		return out;
	}
protected:
	double _time;
	double _slope;
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
double tSample = 1.0/100.0; // default 100Hz
double tEnd = 5;
char stimulusType = 's';

////////////////////////////////////////////////////////////////////////////////

void parseArgs(int argc, char **argv) {
	bool ok = true;
	char c;
	while ((c = getopt (argc, argv, "p:i:d:s:e:f:x:")) != -1) {
		switch (c) {
			case 'p' : kp = atof(optarg); break;
			case 'i' : ki = atof(optarg); break;
			case 'd' : kd = atof(optarg); break;
			case 'f' : lpf = atof(optarg); break;
			case 's' : tSample = atof(optarg); break;
			case 'e' : tEnd = atof(optarg); break;
			case 'x' : stimulusType = optarg[0]; break;
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
			);
	}
}

////////////////////////////////////////////////////////////////////////////////

int main (int argc, char **argv) {

	parseArgs(argc, argv);
	
	PidSim sim;
	sim.setPlant(new Plant());
	
	PidSim::IProcessor* pStim = 0;
	switch(stimulusType) {
		case 'r' : pStim = new Ramp(1,1); break;
		case 'i' : pStim = new Impulse(1); break;		
		case 's' : 
		default:   pStim = new Step(1); break;
	}	
	sim.setStimulus(pStim);
	
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
}

