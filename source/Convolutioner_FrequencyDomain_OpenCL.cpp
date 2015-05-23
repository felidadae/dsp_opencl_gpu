//
//  Convolutioner_FrequencyDomain_OpenCL.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 06.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "Convolutioner_FrequencyDomain_OpenCL.h"
#include "DebugFunctions.h"
#include "FMTypes.h"



#pragma mark Creation

const char Convolutioner_FrequencyDomain_OpenCL::kernelPath_[] = "convolute_kernel_frequency_domain.cl";

//const char Convolutioner_FrequencyDomain_OpenCL::openclProgramMacrodefinitions_[] = "-D float_type=float";// FLOAT_TYPE_STRING;

Convolutioner_FrequencyDomain_OpenCL::Convolutioner_FrequencyDomain_OpenCL():   OpenCLProcessorBase((char*)kernelPath_, true, NULL),
///////////////////////////////////////////////////////////////////////
bufferPIR_R_(context_, cmdQueue_, &lastCommandStatus_),         bufferPIR_I_(context_, cmdQueue_, &lastCommandStatus_),
bufferTransform_R_(context_, cmdQueue_, &lastCommandStatus_),   bufferTransform_I_(context_, cmdQueue_, &lastCommandStatus_),
bufferFDL_R_(context_, cmdQueue_, &lastCommandStatus_),         bufferFDL_I_(context_, cmdQueue_, &lastCommandStatus_),
bufferAccumulator_R_(context_, cmdQueue_, &lastCommandStatus_), bufferAccumulator_I_(context_, cmdQueue_, &lastCommandStatus_),
///////////////////////////////////////////////////////////////////////
bufferGlobalParameters_(context_, cmdQueue_, &lastCommandStatus_)
{
    
    //create Kernel complexMultiplyAdd_kernel_
    complexMultiplyAdd_kernel_ = clCreateKernel(program_, "complexMultiplyAdd", &lastCommandStatus_);
    //.
    
    //Create and set as kernel argument: bufferGlobalParameters
    bufferGlobalParameters_.recreate(CL_MEM_READ_ONLY, 5);
    bufferGlobalParameters_.setAsKernelArgument(6, complexMultiplyAdd_kernel_);
    //.
    
}

Convolutioner_FrequencyDomain_OpenCL::~Convolutioner_FrequencyDomain_OpenCL() {
    if(fftPlan_)
        clFFT_DestroyPlan( fftPlan_ );
}





#pragma mark setNewIR

void Convolutioner_FrequencyDomain_OpenCL::setNewIR(const IRF* irf) {
    irf_ = irf;
    if(window_.get_inputBlockSize() != 0) {
        
        partitionedIR_.setNewIRF( irf_, window_.get_channelLength() );
        
        //Recreate and set buffer: PIR
        //recreate
        bufferPIR_R_.recreate(CL_MEM_READ_ONLY, partitionedIR_.get_partSize() * partitionedIR_.get_numOfPartsPerChannel() * partitionedIR_.get_numOfChannels() );
        bufferPIR_I_.recreate(CL_MEM_READ_ONLY, partitionedIR_.get_partSize() * partitionedIR_.get_numOfPartsPerChannel() * partitionedIR_.get_numOfChannels() );
        //.
        
        //initialize
        bufferPIR_R_.set(partitionedIR_.real_);
        bufferPIR_I_.set(partitionedIR_.real_);
        //.
        //.
    }
}





#pragma mark process

void Convolutioner_FrequencyDomain_OpenCL::process(AudioInOutBuffers<float_type>& audio ) {
    
    //
    unsigned int _2B = audio.channelLength_ * 2;
    unsigned int _B  = audio.channelLength_;
    unsigned int _C  = audio.numOfChannels_;                        //numOfChannels
    unsigned int _P  = partitionedIR_.get_numOfPartsPerChannel();   //numOfIRPartsPerChannel
    //.
    
    //_ if >>>latency<<< or >>>number of channels<<< changed:
    //      set partitionedIR
    //      recreate buffers
    //      recreate fft plans
    if ( window_.get_inputBlockSize() != audio.channelLength_ || window_.get_numOfChannels() != audio.numOfChannels_) {
        
        //Setting partitionedIR
        if (window_.get_inputBlockSize() != audio.channelLength_) {
            
            partitionedIR_.setNewIRF( irf_, audio.channelLength_ );
            _P = partitionedIR_.get_numOfPartsPerChannel();
            
            //Recreate, initialize buffers, and set as kernel arguments: PIR
            //recreate
            bufferPIR_R_.recreate(CL_MEM_READ_ONLY, _2B * _C * _P);
            bufferPIR_I_.recreate(CL_MEM_READ_ONLY, _2B * _C * _P);
            //.
            
            //initialize
            bufferPIR_R_.set(partitionedIR_.real_     );
            bufferPIR_I_.set(partitionedIR_.imaginary_);
            //.
            
            //set as kernel argument
            bufferPIR_R_.setAsKernelArgument(0, complexMultiplyAdd_kernel_);
            bufferPIR_I_.setAsKernelArgument(1, complexMultiplyAdd_kernel_);
            //.
            //.(Recreate...)
            
        }
        //.
        
        //Recreate initialize buffers, and set as kernel arguments: transform, FDL, accumulator
        //recreate
        /****/bufferTransform_R_.recreate(CL_MEM_READ_WRITE,    _2B * _C        );
        /****/bufferTransform_I_.recreate(CL_MEM_READ_WRITE,    _2B * _C        );
        /**********/bufferFDL_R_.recreate(CL_MEM_READ_WRITE,    _2B * _C * _P   );
        /**********/bufferFDL_I_.recreate(CL_MEM_READ_WRITE,    _2B * _C * _P   );
        /**/bufferAccumulator_R_.recreate(CL_MEM_READ_WRITE,    _2B * _C        );
        /**/bufferAccumulator_I_.recreate(CL_MEM_READ_WRITE,    _2B * _C        );
        cpu_bufferAccumulator_R_ = new float_type[_2B * _C ];
        cpu_bufferAccumulator_I_ = new float_type[_2B * _C ];
        //.
        
        //initialize FDL with 0
        bufferFDL_R_.fillWithZero();
        bufferFDL_I_.fillWithZero();
        lastInsertedDelayLineIdx = 0;
        //.
        
        //set as kernel argument
        /**********/bufferFDL_R_.setAsKernelArgument(2, complexMultiplyAdd_kernel_);
        /**********/bufferFDL_I_.setAsKernelArgument(3, complexMultiplyAdd_kernel_);
        /**/bufferAccumulator_R_.setAsKernelArgument(4, complexMultiplyAdd_kernel_);
        /**/bufferAccumulator_I_.setAsKernelArgument(5, complexMultiplyAdd_kernel_);
        //.
        //.(Recreate...)
        
        //Recreate plans
        clFFT_Dim3 dim;
        dim.x = _2B;
        dim.y = 1;
        dim.z = 1;
        fftPlan_ = clFFT_CreatePlan(context_, dim, clFFT_1D, clFFT_SplitComplexFormat, &lastCommandStatus_);
        //.
    }
    
    //update each time bufferGlobalParameters because of incrementing of lastInsertedDelayLineIdx
    /*(_2B, _C, _P, pir_C, FDL_LINE)*/
    cpuData_bufferGlobalParameters_[0] = _2B;
    cpuData_bufferGlobalParameters_[1] = _C;
    cpuData_bufferGlobalParameters_[2] = _P;
    cpuData_bufferGlobalParameters_[3] = irf_->numOfChannels_;
    cpuData_bufferGlobalParameters_[4] = lastInsertedDelayLineIdx;
    
    bufferGlobalParameters_.set(cpuData_bufferGlobalParameters_);
    //.
    
    //Update channelsWindow
    window_.update( audio, /*history size*/ _B );
    //.
    
    //Init >>bufferTransform<<
    bufferTransform_R_.set(window_.buffer_.data_);
    for(unsigned int i = 0; i < _2B * _C; ++i)
        cpu_bufferAccumulator_I_[i]=0;
    bufferTransform_I_.set(cpu_bufferAccumulator_I_);
    //.
    
    //Make fft of bufferTransform
    lastCommandStatus_ = clFFT_ExecutePlannar(  cmdQueue_, fftPlan_, _C, clFFT_Forward,
                                              bufferTransform_R_, bufferTransform_I_,
                                              bufferTransform_R_, bufferTransform_I_,
                                              0, NULL, NULL );
    //.
    
    //Copy bufferTransform into bufferFDL (inserting new delay line) (real and imaginary part)
    clEnqueueCopyBuffer(    cmdQueue_, bufferTransform_R_, bufferFDL_R_,
                        0, lastInsertedDelayLineIdx * (_2B * _C ) * sizeof(float_type), (_2B * _C ) * sizeof(float_type),
                        0, NULL, NULL);
    clEnqueueCopyBuffer(    cmdQueue_, bufferTransform_I_, bufferFDL_I_,
                        0, lastInsertedDelayLineIdx * (_2B * _C ) * sizeof(float_type), (_2B * _C ) * sizeof(float_type),
                        0, NULL, NULL);
    //.
    
    //Increment host lastInsertedDelayLine
    lastInsertedDelayLineIdx = (lastInsertedDelayLineIdx + 1 ) % _P;
    //.
    
    //Execute kernel
    size_t globalWorkSize[1];
    globalWorkSize[0] =  _2B * _C /* == window_.get_allLength() */;
    lastCommandStatus_ = clEnqueueNDRangeKernel(cmdQueue_, complexMultiplyAdd_kernel_, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    if(lastCommandStatus_ == -4) {
        std::cout << "Too much amount of memory must be allocated on the GPU due to lenght of impulse response and number of channels.";
        throw int();
    }
    else if(lastCommandStatus_ != 0) {
        std::cout << "Error while sending clEnqueueNDRangeKernel.";
        throw int();
    }
    //.
    
    //ifft of bufferAccumulator
    lastCommandStatus_ = clFFT_ExecutePlannar(  cmdQueue_, fftPlan_, _C, clFFT_Inverse,
                                              bufferAccumulator_R_, bufferAccumulator_I_,
                                              bufferAccumulator_R_, bufferAccumulator_I_,
                                              0, NULL, NULL );
    //.
    
    //Copy from bufferAccumulator to cpu
    bufferAccumulator_R_.get(cpu_bufferAccumulator_R_);
    //.
    
    //Flushing and finishing
    clFlush(cmdQueue_);
    clFinish(cmdQueue_);
    //.
    
    //Write fftw vector form to audio.outputChannel[number of Channel]
    for (unsigned int channNum = 0; channNum < _C; ++channNum)
        for (unsigned sampleNum = 0; sampleNum < _B; ++sampleNum)
            audio.out_[channNum][sampleNum] = (cpu_bufferAccumulator_R_[channNum*_2B + _B + sampleNum])/_2B;
    //.
}










