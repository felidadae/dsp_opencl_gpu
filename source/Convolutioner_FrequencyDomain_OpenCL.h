//
//  Convolutioner_FrequencyDomain_OpenCL.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 06.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __LowPassFilter_Standalone__Convolutioner_FrequencyDomain_OpenCL__
#define __LowPassFilter_Standalone__Convolutioner_FrequencyDomain_OpenCL__

#include "ConvolutionerBase.h"
#include "OpenCLProcessorBase.h"
#include "OpenCLBuffer.h"
#include "PIR_fftw.h"
//---
//
#include "clFFT.h"


class Convolutioner_FrequencyDomain_OpenCL: public ConvolutionerBase, public OpenCLProcessorBase {
public:
    
    Convolutioner_FrequencyDomain_OpenCL();
    ~Convolutioner_FrequencyDomain_OpenCL();
    
    void setNewIR(const IRF* irf);
    
    void process(AudioInOutBuffers<float_type>& audio);
    
    
private:
    //Partitioned impulse response in frequency domain
    PIR_fftw partitionedIR_;
    //.
    
    //fft plan
    clFFT_Plan fftPlan_ = nullptr;
    //.
    
    //fdl
    unsigned int lastInsertedDelayLineIdx = 0;
    //.
    
    //Buffers
    unsigned int cpuData_bufferGlobalParameters_[5];
    //---
    OpenCLBuffer<float_type>            bufferPIR_R_,         bufferPIR_I_;
    OpenCLBuffer<float_type>      bufferTransform_R_,   bufferTransform_I_;
    OpenCLBuffer<float_type>            bufferFDL_R_,         bufferFDL_I_;
    OpenCLBuffer<float_type>    bufferAccumulator_R_, bufferAccumulator_I_;
    float_type*                 cpu_bufferAccumulator_R_, *cpu_bufferAccumulator_I_;
    OpenCLBuffer<cl_uint>       bufferGlobalParameters_;
    //.
    
    //Kernel
    static const char kernelPath_[];
    //static const char openclProgramMacrodefinitions_[];
    cl_kernel complexMultiplyAdd_kernel_;
    //.
    
};


#endif /* defined(__LowPassFilter_Standalone__Convolutioner_FrequencyDomain_OpenCL__) */
