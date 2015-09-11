#ifndef _STIMULUS_INCLUDED
#define _STIMULUS_INCLUDED

#include "pidsim.h"

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

#endif
