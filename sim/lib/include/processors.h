#ifndef _PROCESSORS_INICLUDED
#define _PROCESSORS_INICLUDED


#include <stdlib.h> // for rand()

// base class for all processors with a single input 
class SingleInputProcessor : public IProcessor
{
public:
	SingleInputProcessor() : _input(0) {}
	void setInput(ISource* s) { _input = s; }
protected:
	ISource* _input;
};


// gain
class Gain: public SingleInputProcessor
{
public:
	Gain(double gain = 1): _gain(gain) {}
	void setGain(double gain) { _gain = gain; }
	// IProcessor
	virtual void update(double time) { 
		_output = _input->get() * _gain; 
	}
private:
	double _gain;
};


#include <vector>

// abstract multi-input processor
class MultiInputProcessor : public IProcessor
{
public:
	void addInput(ISource* p) { _inputs.push_back(p); }
protected:
	typedef std::vector<ISource*> Inputs;
	Inputs _inputs;
};


// sums all the inputs
class Summer: public MultiInputProcessor
{
public:
	Summer() {}
	virtual void update(double time) {
		_output = 0;
		for( Inputs::const_iterator i = _inputs.begin();
			 i != _inputs.end(); i++ ) {
			ISource* p = *i;
			_output += p->get();
		 }
	}
};


// integrator (unclamped, may go to infinity)
class Integrator: public SingleInputProcessor
{
public: 
	Integrator() : _accumulator(0), _prevTime(0)
	{}

	virtual void update(double time) {
		double deltaTime = time - _prevTime;
		_accumulator = _accumulator + _input->get() * deltaTime;
		_output = _accumulator;
		_prevTime = time;
	}

private:
	double _accumulator;
	double _prevTime;
};


// differentiator
class Differentiator: public SingleInputProcessor
{
public: 
	Differentiator() : _prevTime(0), _prevInput(0)
	{}

	virtual void update(double time) {
		double deltaTime = time - _prevTime;
		double derivative = ( deltaTime == 0 ) ? 
			0 : (_input->get() - _prevInput) / deltaTime;
		_output = derivative;
		_prevTime = time;
		_prevInput = _input->get();
	}

private:
	double _prevTime;
	double _prevInput;
};

// adds noise to input signal
class NoiseAdder: public SingleInputProcessor
{
public:
	NoiseAdder(double amplitude): _amplitude(amplitude) {}
	virtual void update(double time) {
		double input = _input->get();
		double noise = _amplitude * double(rand()) / double(RAND_MAX);
		_output = input + noise;
	}
private:
	double _amplitude;
};


// specialized error processor
// produces error value = target - feedback
// it could have been implemented as a MultiInput processor but...
class ErrorProcessor: public IProcessor
{
public:
	ErrorProcessor() {}
	void setFeedbackInput(ISource* pS) { _feedback = pS; }
	void setTargetInput(ISource* pS) { _target = pS; }
	// IProcessor
	virtual void update(double time) { 
		_output = _target->get() - _feedback->get(); 
	}
private:
	ISource* _feedback;
	ISource* _target;
};


#endif
