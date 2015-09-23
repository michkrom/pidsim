# FIR 

FIR filter implementation, builder (coeff computation) and tester.

Overview
----------

The FIR filter implementation is templetized on <T> - sample time and <TC> - internal compute time.
One can use integral or floating point (FP) samples. The computation can be FP or a integral computation.
However, with integral computation an overflow is a potential (due to multiply-and-accumulate) hence the internal type should have much more range then sample range (e.g. uint16_t for samples and uint32_t for TC).

The FIR implements also a filter configurators for standard set of filters: boxcar average, low-pass, hi-pass, band-pass, band-stop with ability to specify windowing function. Two windowing functions are provided: Hann and Hamming.

Examples (from main.cpp):
	// create 65 tap FIR
    TFir<double, 65> fir;
    
	// upsampling
    fir.lowpass(.4); // low pass
    Step<double> s;
    Upsample<double> src(2, &s);
    run(src, fir, 128);

	// hi pass with Hamming window
    fir.hipass(0.125); //, 1, fir.Hamming); // high pass
    Step<double> src;
    Link<double> src(&stp, &nse);
    run(src, fir, 128);
    
    // low pass to filter the noise
    fir.lowpass(0.0625,fir.Hamming);
    Step<double> stp;
    Noise<double> nse;
    Link<double> src(&stp, &nse);
    run(src, fir, 128);
    
TODO
-----
- combine projects in such a way the components can be shared/interchanged; specifically standaraze on IDsp, IProcessor etc.

BUILD
------

'fir' still uses simple makefile so just 'make' would suffice.
