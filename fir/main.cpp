#include <stdio.h>
#include <stdlib.h>

#include "Fir.h"

///////////////////////////////////////////////////////////////////////////////

// upsamples holds original data source and zero-stuffs its output
template<class T>
class Upsample : public IDsp<T> {
public:
    Upsample(int ratio, IDsp<T>* src) : _src(src), _cnt(0), _ratio(ratio) {}

    virtual void put(T sample) { /*ignored*/ }
    virtual T get() { 
        return _cnt++ % _ratio == 0 ? _src->get() : 0;
    }

private:
    IDsp<T>* _src;
    int _cnt;
    int _ratio;
};

///////////////////////////////////////////////////////////////////////////////

// adds a step to incomming signal (defaults to 0)
template<class T, class TC = double>
class Step : public IDsp<T> {
public:
    Step(TC ampl = 1) : _sample(0), _ampl(ampl) {}

    virtual void put(T sample) { _sample = sample; }
    virtual T get() { return T(_sample + _ampl); }

private:
    T _sample;
    TC _ampl;
};

///////////////////////////////////////////////////////////////////////////////

template<class T, class TC = double>
class Noise : public IDsp<T> {
public:
    Noise(double gain = 1) : _sample(0), _gain(gain) {}

    virtual void put(T sample) { _sample = sample; }
    virtual T get() { return T(_sample + TC(_gain * rand() - _gain/2) / TC(RAND_MAX)); }

private:
    T _sample;
    TC _gain;
};


///////////////////////////////////////////////////////////////////////////////

// links in chain two IDsps (passes data from source to sink)
template<class T, class TC = double>
class Link : public IDsp<T> {
public:
    Link(IDsp<T>* src, IDsp<T>* snk) : _src(src), _snk(snk) {}

    virtual void put(T sample) { /* ignored */ }
    virtual T get() { _snk->put( _src->get() );  return _snk->get(); }

private:
    IDsp<T>* _src;
    IDsp<T>* _snk;
};

///////////////////////////////////////////////////////////////////////////////

#include <vector>
template<class T>
class Sink: public IDsp<T> {
public:
    Sink() {}
    virtual void put(T sample) { _store.push_back(sample); }
    virtual T get() { return _store.pop_front(); }
    const std::vector<T>& store() { return _store; }
private:
    std::vector<T> _store;
};

///////////////////////////////////////////////////////////////////////////////

template<class T>
void run(IDsp<T>& src, IDsp<T>& dsp, int n)
{
    for (int i = 0; i < n; i++)
    {
        T sample = src.get();
        dsp.put(sample);
        printf("%u %f %f\n", i, sample, dsp.get());
    }
    printf("\n");
}

///////////////////////////////////////////////////////////////////////////////

int main()
{
    TFir<double, 65> fir;
#if 0 // upsampling
    fir.lowpass(.4); // low pass
    //fir.boxcar();
    //NoiseAdder<double> src(1);
    Step<double> s;
    Upsample<double> src(2, &s);
#endif
#if 0
    fir.hipass(0.125); //, 1, fir.Hamming); // high pass
    Step<double> src;
#endif
#if 1
    fir.lowpass(0.0625,fir.Hamming);
    Step<double> stp;
    Noise<double> nse;
    Link<double> src(&stp, &nse);
#endif
    run(src, fir, 128);
}

