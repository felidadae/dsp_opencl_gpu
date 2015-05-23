//
//  PIROpenCL.h
//  DSPOpenCLGPUTester
//
//  Created by kadlubek47 on 07.10.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __DSPOpenCLGPUTester__PIROpenCL__
#define __DSPOpenCLGPUTester__PIROpenCL__

#include "PIRBase.h"
#include "IRF.h"
#include "AudioBuffer.h"
#include "OpenCLBuffer.h"
#include "clFFT.h"
#include <cmath>


//Using that class results in unkown problems with OpenCL implementation
class PIROpenCL: public PIRBase {
public:
    PIROpenCL(cl_context& context, cl_command_queue& cmdQueue, cl_int* lastCommandStatus);
    
    void setNewIRF(const IRF* irf, unsigned int blockSize);
    
    OpenCLBuffer<float_type> bufferImaginary_;
    OpenCLBuffer<float_type> bufferReal_;
    
    unsigned int get_numOfChannels() { return numOfChannels_; }
    unsigned int get_channelLength() { return channelLength_; }
    
private:
    float_type* imaginary_ = nullptr; // [[PIR(chann=0, part=0)] [PIR(chann=1, part=0)] ... [PIR(chann=N-1, part=0)]]  [ {PIR(chann={0:N-1}, part = 1)} ] ... [PIR(chann={0:N-1}, part = numOfPartsIRPerChannel)]
    float_type* real_      = nullptr;
    
    //OpenCL environment
    cl_context&         context_;
    cl_command_queue&   cmdQueue_;
    cl_int*            lastCommandStatus_;
    //.
};



#endif /* defined(__DSPOpenCLGPUTester__PIROpenCL__) */
