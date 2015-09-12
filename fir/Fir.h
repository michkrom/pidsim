#pragma once
//
// a simple FIR filter implementation
// templetized on T - sample type and TC compute type
// note that when TC is integral, set RANGE to max sample value!
//
#define _USE_MATH_DEFINES
#include <math.h>

// a generic inteface for getting and putting samples into a DSP 'processor'
// T - sample type
template<class T>
class IDsp {
public: 
    virtual void put(T sample) = 0;
    virtual T get() = 0;
};


// FIR filter implemenation
// T - sample type, 
// N - filter length, 
// TC - compute type (default to T)
// RANGE - filter computational of coeffitients for use with fixpoint math
template<class T, int N, class TC = T, int RANGE = 1>
class TFir : public IDsp<T>
{
public:
    TFir() : 
        _idx(0)
    {
        for (int n = 0; n < N; n++) _samples[n] = 0;
    }

    ~TFir() {
    }

    void put(T sample) { 
        _samples[_idx] = sample;
        _idx = (_idx + 1 ) % N;
    }

    T get() {
        TC sum = 0;
        for (int i = 0, j = _idx == 0 ? N-1 : _idx-1; 
                i < N; 
                i++, j++) 
        {
            sum += _coeffs[i] * TC(_samples[j % N]);
        }
        return T(sum);
    }

    int length() const { return N; }

    void setCoeffs(TC* coeffs) { 
        for (int i = 0; i < N; i++)
            _coeffs[i] = coeffs[i];
    }


    TC coeff(unsigned i) const { return _coeffs[i]; }
    TC* coeffs() const { return _coeffs; }

    // http://www.labbookpages.co.uk/audio/firWindowing.html
    // http://www.mikroe.com/chapters/view/72/chapter-2-fir-filters/

    //  configurators use normalized frequency = frequencyHZ / sampleRateHz


    // boxcar average
    void boxcar() {
        for (int n = 0; n < N; n++)
            _coeffs[n] = RANGE / N;
    }

    // triangular 
    void triangular() {
        for (int n = 0; n < N; n++) {
            _coeffs[n] = RANGE *
                (n < (N - 1) / 2 ?
                2 * TC(n) / TC(N - 1) :
                2 - 2 * TC(n) / TC(N - 1));
        }
    }

    // lowpas -> sin(x)/x * window
    void lowpass(double cutoffFrequency, double (*windowFunc)(unsigned) = 0) {
        const unsigned M = (N - 1)/2;
        const double w = 2 * cutoffFrequency;
        for (int n = 0; n < N; n++) {
            double x = M_PI * (double(n) - M);
            double c =
                n == M ?
                    2 * cutoffFrequency :
                    sin(w * x) / x;
            c *= windowFunc ? windowFunc(n) : 1.0;
            _coeffs[n] = TC(c * RANGE);
        }
    }

    // hipass sin(x)/x * window
    void hipass(double cutoffFrequency, double(*windowFunc)(unsigned) = 0) {
        const unsigned M = (N - 1)/2;
        const double w = 2 * cutoffFrequency;
        for (int n = 0; n < N; n++) {
            double x = M_PI * (double(n) - M);
            double c =
                n == M ?
                    1 - 2 * cutoffFrequency :
                    - sin(w * x) / x;
            c *= windowFunc ? windowFunc(n) : 1.0;
            _coeffs[n] = TC(c * RANGE);
        }
    }

    // N must be odd!
    void bandpass(double frequency1, double frequency2, double(*windowFunc)(unsigned) = 0) {
        const unsigned M = (N - 1)/2;
        const double w1 = 2 * M_PI * frequency1;
        const double w2 = 2 * M_PI * frequency2;
        for (int n = 0; n < N; n++) {
            double c =
                n == M ?
                    2 * (frequency2-frequency1) :
                    (sin(w2 * (n - M)) - sin(w1 * (n - M))) / (M_PI*(n - M));
            c *= windowFunc ? windowFunc(n) : 1.0;
            _coeffs[n] = TC(c * RANGE);
        }
    }

    // N must be odd!
    void bandstop(double frequency1, double frequency2, double(*windowFunc)(unsigned) = 0) {
        const unsigned M = N - 1;
        for (int n = 0; n < N; n++) {
            double c =
                n == M / 2 ?
                    1 - 2 * (frequency2 - frequency1) :
                    (sin(2 * M_PI * frequency1 * (n - M / 2)) - sin(2 * M_PI * frequency2 * (n - M / 2))) / (M_PI*(n - M / 2));
            c *= windowFunc ? windowFunc(n) : 1.0;
            _coeffs[n] = TC(c * RANGE);
        }
    }

    // windowing functions - 'n' is filter coeff #

    static double Hann(unsigned n) {
        return  n < (N - 1) / 2 ?
            2.0 * double(n) / (N - 1) :
            2.0 - 2.0 * double(n) / (N - 1);
    }

    static double Hamming(unsigned n) {
        return 0.54 - 0.46 * cos(2.0 * M_PI * n / (N - 1));
    }

private:
    T _samples[N];
    TC _coeffs[N];
    unsigned _idx;
};
