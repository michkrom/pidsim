////////////////////////////////////////////////////////////////////////////////
// PID controller simulator
//

#include "pidsim.h"

////////////////////////////////////////////////////////////////////////////////
// LPF computation  (first order LP filter)
static double lpf(double input, double prevOutput, double ts, double tau)
{
	double alpha = tau / (tau + ts);
	double out = alpha * prevOutput + (1-alpha) * input;
	return out;
}

////////////////////////////////////////////////////////////////////////////////

PidSim::PidSim() :
	_kp(1), _ki(0), _kd(0), _tau(0),
	_prevTime(0),
	_prevError(0),
	_stimulus(0),
	_error(0),
	_control(0),
	_feedback(0),
	_integral(0),
	_kdf(0),
	_pplant(0), 
	_pstimulus(0)
{}

////////////////////////////////////////////////////////////////////////////////

void PidSim::setLPF(double cutoffFreq) {
	_tau = cutoffFreq > 0 ? 1 / ( 2 * PI * cutoffFreq ) : 0;
}

////////////////////////////////////////////////////////////////////////////////
// advances simulation state with a time step (next sample)
//
void PidSim::step(double time)
{
	// this is sample time (for this sample)
	double deltaTime = time - _prevTime;
	_prevTime = time;
	
	if( deltaTime > 0 ) {
		// compute stimulus value (stimulus ignores input hence set to 0)
		_stimulus = _pstimulus ? _pstimulus->compute(0,time) : 0;
		
		// error signal
		_error = _stimulus - _feedback;

		// PID controller
		double derivative = (_error - _prevError) / deltaTime;
				
		_integral = _integral + _error * deltaTime;
		
		// lpf in D path
		_kdf = lpf(_kd * derivative, _kdf, deltaTime, _tau);

		_control = _kp * _error + _ki * _integral + _kdf;
		
		// use the supplied plant to compute feedback
		_feedback = _pplant ? _pplant->compute( _control, time ) : 0;
	}
	else if( time == 0 ) { 
		// initial conditions
		_stimulus = 0;
		_error = 0;
		_integral = 0;
		_kdf = 0;
		_control = 0;
		_feedback = 0;
	}
}

