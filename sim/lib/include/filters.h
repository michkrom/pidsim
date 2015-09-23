#ifndef _FILTERS_INCLUDED
#define _FILTERS_INCLUDED

#include <deque>
#include <numeric> 

// simple, not optimal implementation of moving average
class MovingAverage : SingleInputProcessor
{
public:
	MovingAverage(unsigned length) : _length(length) {}
	virtual void update(double time) {
		_fifo.push_back(_input->get());
		if( _fifo.size() > _length )
			_fifo.pop_front();
		_output = std::accumulate(_fifo.begin(), _fifo.end(), 0);
	}
private:
	std::deque<double> _fifo;
	unsigned _length;
};


// first order low pass filter
// Tau = 1 / (2*PI*Fcutoff)
class FirstOrderLowPass: public SingleInputProcessor
{
public:
	FirstOrderLowPass(double timeConst) : 
		_prevTime(0),
		_timeConst(timeConst)
	{}
		
	virtual void update(double time) {
		double ts = time - _prevTime;
		double alpha = _timeConst / (_timeConst + ts);
		double input = _input->get();
		_output = alpha * _output + (1-alpha) * input;
		_prevTime = time;
	}
private:
	double _prevTime;
	double _timeConst;
};

#endif
