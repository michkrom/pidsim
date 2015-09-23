#ifndef _IPROCESSOR_INCLUDED
#define _IPROCESSOR_INCLUDED

// source of a value
class ISource
{
public:
	virtual double get() = 0;
};


// a processor is a node of simulation: 
// - it can process simulation step and 
// - it implements ISource so it can provide an output value
// note - this is more then just interface as it provides some rudimetary
// behavior, notably maitainging _output state and implementing ISource::get()
class IProcessor: public ISource
{
public:
	//update simulation step with absolute "time"
	virtual void update(double time) = 0;

	// ISource
	virtual double get() { return _output; }
	
	virtual ~IProcessor() {}
	
protected:
	double _output;
	IProcessor() : _output(0) {}
};

#endif
