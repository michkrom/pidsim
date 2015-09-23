#ifndef _PIDSIM_INCLUDED_
#define _PIDSIM_INCLUDED_

////////////////////////////////////////////////////////////////////////////////
// PID controller simulator
//

class PidSim
{
public:
	PidSim();
	
	// interface to a plant and stimulus model, supplied by user
	class IProcessor {
		// processes input into output
		public: virtual double compute(double input, double time) = 0;
	};
	
	// sets user's plant
	void setPlant(IProcessor* p) { _pplant = p; }
	// sets user's stimulus
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

// could use M_PI from math but support varies
#define PI           3.14159265358979323846  /* pi */

#endif
