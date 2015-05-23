//
//  CopyingProcessor_OpenCL.cpp
//  GPUProcessingTesting
//
//  Created by kadlubek47 on 17.06.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "CopyingProcessor_OpenCL.h"



#pragma mark Creation, destroying

const char CopyingProcessor_OpenCL::kernelPath_[] = "simply_kernels.cl";

CopyingProcessor_OpenCL::CopyingProcessor_OpenCL(): OpenCLProcessorBase((char*)kernelPath_) {
    inOutKernel_ = clCreateKernel(program_, "in_out", &lastCommandStatus_);
}

CopyingProcessor_OpenCL::~CopyingProcessor_OpenCL() {
    
    //clear
    for(unsigned int channNum = 0; channNum < buffersIn_.size(); ++channNum) {
        delete /***/buffersIn_[channNum];
        delete /**/buffersOut_[channNum];
    }
    /***/buffersIn_.clear();
    /**/buffersOut_.clear();
    //.
    
    clReleaseKernel(inOutKernel_);
}



#pragma mark manyChannel main_function

void CopyingProcessor_OpenCL::process(AudioInOutBuffers<float>& audio) {
    
    //Check if recreate
    if ( audio.numOfChannels_ != buffersIn_.size() || audio.channelLength_ != buffersIn_[0]->getLength() ) {
        
        //clear
        for(unsigned int channNum = 0; channNum < buffersIn_.size(); ++channNum) {
            delete /***/buffersIn_[channNum];
            delete /**/buffersOut_[channNum];
        }
        /***/buffersIn_.clear();
        /**/buffersOut_.clear();
        //.
        
        for(unsigned int channNum = 0; channNum < audio.numOfChannels_; ++channNum) {
            /***/buffersIn_.push_back( new OpenCLBuffer<float>(context_, cmdQueue_, &lastCommandStatus_) );
            /**/buffersOut_.push_back( new OpenCLBuffer<float>(context_, cmdQueue_, &lastCommandStatus_) );
            /***/buffersIn_.back()->recreate(CL_MEM_READ_ONLY,  audio.channelLength_);
            /**/buffersOut_.back()->recreate(CL_MEM_WRITE_ONLY, audio.channelLength_);
        }
    }
    //.
    
    //Transfer to GPU
    for(unsigned int channNum = 0; channNum < audio.numOfChannels_; ++channNum)
        buffersIn_[channNum]->set(audio.in_[channNum]);
    //.
    
    //Execute kernel
    size_t globalWorkSize[1];
    globalWorkSize[0] = audio.channelLength_;
    //---
    for(unsigned int channNum = 0; channNum < audio.numOfChannels_; ++channNum) {
        /***/buffersIn_[channNum]->setAsKernelArgument(0, inOutKernel_);
        /**/buffersOut_[channNum]->setAsKernelArgument(1, inOutKernel_);
        lastCommandStatus_ = clEnqueueNDRangeKernel( cmdQueue_, inOutKernel_, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    }
    //.
    
    //Transfer from GPU to CPU
    for(unsigned int channNum = 0; channNum < audio.numOfChannels_; ++channNum)
        buffersOut_[channNum]->get(audio.out_[channNum]);
    //.
    
    //Flushing and finishing
    clFlush(cmdQueue_);
    clFinish(cmdQueue_);
    //.
}

