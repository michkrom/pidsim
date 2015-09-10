#ifndef _PIDPROCESSOR_INCLUDED
#define _PIDPROCESSOR_INCLUDED

#include "processors.h"


// simple PID controller
class PIDProcessor: public SingleInputProcessor
{
public: 
	PIDProcessor(): 
		_kp(1),_ki(0),_kd(0), 
		_prevError(0), 
		_prevTime(0),
		_integral(0)
	{}

	virtual void update(double time) {
		double error = _input->get();
		double deltaTime = time - _prevTime;
		double derivative = ( deltaTime == 0 ) ? 
			0 : (error - _prevError) / deltaTime;
		_integral = _integral + error*deltaTime;
		_output = _kp*error + _ki*_integral + _kd*derivative;	
		_prevError = error;
		_prevTime = time;
	}

	void setKP(double kp) { _kp = kp; }
	void setKI(double ki) { _ki = ki; }
	void setKD(double kd) { _kd = kd; }

private:
	double _kp;
	double _ki;
	double _kd;
	double _prevError;
	double _prevTime;
	double _integral;
};

#endif
