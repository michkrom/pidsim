#ifndef _PIDSIM_INCLUDED_
#define _PIDSIM_INCLUDED_

////////////////////////////////////////////////////////////////////////////////
// PID controller simulator
//

class PidSim
{
public:
	PidSim();
	
	// interface to a plant model, supplied by user
	class IProcessor {
		// computes feedback signal from plant
		public: virtual double compute(double input, double time) = 0;
	};
	
	// sets users plant
	void setPlant(IProcessor* p) { _pplant = p; }
	// sets users plant
	void setStimulus(IProcessor* p) { _pstimulus = p; }
	
	// PID gains
	void setKp(double v) { _kp = v; }
	void setKi(double v) { _ki = v; }
	void setKd(double v) { _kd = v; }
	// LPF cut off frequency in D branch
	void setLPF(double freq);
	
	// simulation state getters
	double getStimulus() const { return _stimulus; }
	double getError() const { return _error; }
	double getControl() const { return _control; }
	double getFeedback() const { return _feedback; }
	
	// compute simulation step given absolute time
	void step(double time);
	
private:
	double _kp;
	double _ki;
	double _kd;
	double _tau;
	
	double _prevTime;
	double _prevError;
	
	double _stimulus;
	double _error;
	double _control;
	double _feedback;
	
	double _integral;
	double _kdf;
	
	IProcessor* _pplant;
	IProcessor* _pstimulus;
	
};

#define PI           3.14159265358979323846  /* pi */

#endif
