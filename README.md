#dsp_opencl_gpu_tester
Check on what condition GPU can be used in digital audio processing.

Written the simplest kernel which just copy input to output.

Written real-time FFT convolutioner.



>>Execution of program<<
File "in.txt" should be put where executable file is.
Write in "in.txt" if experiments should be carried or if only one processing 
of audio file and given impulse response should be made. Set value of field 
IF_CARRY_EXPERIMENTS to YES to carry experiments, NO to make only one 
processing. If set to NO, give a proper size of block and choose where should 
be made computations: on CPU or on GPU. Result of single processing will be put
in folder "singleProcessing". Each experiment which is defined in class Laboratory
will have seperate folder.

>>Migrating on different platforms<<
Used libraries: 
OpenCL implementation,
fftw (float version),
libsndfile.
boost library.
Folders are created by mkdir function which will work on all POSIX systems.
For windows it must be changed for aprioprate function.

>>Notes concerning FilterModule<<
Only 32bit float supported;
There is no many things to change to support 64bit float but firstly of course 
GPU must support 64bit float processing:
1) in file FMTypes float_type define as double
2) change functions names from fftw library from "fftwf" to "fftw" )
3) change kernel arguments from float -> double)
4) modify opencl fft apple calls (see sample code: file main.cpp there should
be answer how to do it)
