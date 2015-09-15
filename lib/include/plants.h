#ifndef _PLANTS_INCLUDED
#define _PLANTS_INCLUDED

#include "processors.h"

// output = input
class SimplePlant: public SingleInputProcessor
{
public:
	SimplePlant() {}
	virtual void update(double time) {
		_output = _input->get();
	}
};

// returns average of previous input and current input
class AveragingPlant: public SingleInputProcessor
{
public:
	AveragingPlant() {}
	virtual void update(double time) {
		_output = ( _prevInputV + _input->get() ) / 2;
		_prevInputV = _input->get();
	}
private:
	double _prevInputV;
};

#endif
