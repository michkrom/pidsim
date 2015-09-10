#ifndef _STIMULUS_INCLUDED
#define _STIMULUS_INCLUDED


#include "IProcessor.h"

// DC - produces a constant value
class DCStimulus: public IProcessor
{
public:
	DCStimulus(double dc) { _output = dc; }
	virtual void update(double t) {}	
};


// step stimulus produces a step at given time
class StepStimulus: public IProcessor
{
public:
	StepStimulus(double stepTime, double amplitude = 1) : 
		_stepTime(stepTime),
		_amplitude(amplitude)
	{}
	
	virtual void update(double time) { 
		_output = time >= _stepTime ? _amplitude : 0; 
	}
	
private:
	double _stepTime;
	double _amplitude;
};


// impulse stimulus produces a single sample of 1 when its time is due
class ImpulseStimulus: public IProcessor
{
public:
	ImpulseStimulus(double impulseTime, double amplitude = 1) : 
		_impulseTime(impulseTime), 
		_amplitude(amplitude),
		_fired(false)
		{}
		
	// IProcessor
	virtual void update(double time) { 
		_output = time >= _impulseTime && !_fired ? _amplitude : 0;
		_fired = _fired || _output != 0;
	}
	
private:
	double _impulseTime;
	double _amplitude;
	bool _fired;
};


#endif
