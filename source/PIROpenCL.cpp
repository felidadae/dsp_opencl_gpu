//
//  PIROpenCL.cpp
//  DSPOpenCLGPUTester
//
//  Created by kadlubek47 on 07.10.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "PIROpenCL.h"



PIROpenCL::PIROpenCL(cl_context& context, cl_command_queue& cmdQueue, cl_int* lastCommandStatus): context_(context), cmdQueue_(cmdQueue), lastCommandStatus_(lastCommandStatus), bufferImaginary_(context, cmdQueue, lastCommandStatus), bufferReal_(context, cmdQueue, lastCommandStatus) {}

void PIROpenCL::setNewIRF(const IRF* irf, unsigned int blockSize) {
    
    //Setting
    numOfChannels_              = irf->numOfChannels_;
    partSize_                   = 2 * blockSize;
    numOfPartsPerChannel_       = ceil((float_type)irf->N_ / (float_type)blockSize);
    channelLength_              = numOfPartsPerChannel_ * partSize_;
    unsigned int dataLength     = numOfChannels_ * channelLength_;
    real_                       = new float_type [dataLength];
    imaginary_                  = new float_type [dataLength];
    //---
    /***/bufferReal_.recreate(CL_MEM_READ_ONLY, dataLength );
    bufferImaginary_.recreate(CL_MEM_READ_ONLY, dataLength );
    //.
    
    //FFT Plan
    clFFT_Plan fftPlan;
    clFFT_Dim3 dim;
    dim.x = partSize_;
    dim.y = 1;
    dim.z = 1;
    fftPlan = clFFT_CreatePlan(context_, dim, clFFT_1D, clFFT_SplitComplexFormat, lastCommandStatus_);
    //.
    
    //Init Real and Imaginary part (each filter part has half samples from ir and half 0)
    //Real
    for(unsigned int channNum = 0; channNum < numOfChannels_; ++channNum) {
        
        unsigned int partNum;
        
        //parts 0,1... numOfPartsPerChannel_-2
        for(partNum = 0; partNum < numOfPartsPerChannel_ - 1; ++partNum) {
            memcpy      (real_ + channNum*partSize_ + partNum*partSize_*numOfChannels_ ,          irf->h_[channNum] + partNum*/*BLOCK_SIZE*/blockSize,            sizeof(float_type) * blockSize );
            std::fill_n (real_ + channNum*partSize_ + partNum*partSize_*numOfChannels_ + blockSize,   blockSize, 0);
        }
        //.
        
        //part numOfPartsPerChannel_-1 (last)
        unsigned int numOfSamplesFromIrInLastPart = (irf->N_%blockSize);
        memcpy      (real_ + channNum*partSize_ + partNum*partSize_*numOfChannels_,           irf->h_[channNum] + partNum*blockSize,          sizeof(float_type) * numOfSamplesFromIrInLastPart );
        std::fill_n (real_ + channNum*partSize_ + partNum*partSize_*numOfChannels_ + numOfSamplesFromIrInLastPart,   partSize_ - numOfSamplesFromIrInLastPart, 0);
        //.
        
    }
    //.
    
    //Imaginary
    for(unsigned int sampleNum = 0; sampleNum < dataLength; ++sampleNum)
        imaginary_[sampleNum] = 0;
    //.
    //.
    
    //Send data to GPU
    /***/bufferReal_.set(/***/real_);
    bufferImaginary_.set(imaginary_);
    //.
    
    //Execute
    *lastCommandStatus_ = clFFT_ExecutePlannar(  cmdQueue_, fftPlan, numOfPartsPerChannel_*numOfChannels_, clFFT_Forward,
                                               bufferReal_, bufferImaginary_,
                                               bufferReal_, bufferImaginary_,
                                               0, NULL, NULL );
    //.
    
    //Delete if created before
    delete [] real_;
    delete [] imaginary_;
    //.
    
    //Destroy plan
    clFFT_DestroyPlan( fftPlan );
    //.
}