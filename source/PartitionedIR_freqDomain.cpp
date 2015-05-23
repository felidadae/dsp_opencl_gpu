//
//  PartitionedIR_freqDomain.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 27.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "PartitionedIR_freqDomain.h"
#include <cstring>
#include <algorithm>
//---
#include "DebugFunctions.h"
#include <iostream>



#pragma mark fftw

void PartitionedIR_freqDomain::setNewIRF(const IRF* irf, unsigned int blockSize) {
    
    //Delete if created before
    if(real_.data_) {
        delete [] real_;
        delete [] imaginary_;
    }
    //.
    
    //Setting
    numOfChannels_              = irf->numOfChannels_;
    partSize_                   = 2 * blockSize;
    numOfPartsPerChannel_       = ceil((float_type)irf->N_ / (float_type)blockSize);
    channelLength_              = numOfPartsPerChannel_ * partSize_;
    unsigned int dataLength     = numOfChannels_ * channelLength_;
    real_                       = new float_type [dataLength];
    imaginary_                  = new float_type [dataLength];
    //.
    
    //FFT Plan
    fftwf_iodim dim = {(int) (partSize_),1,1};
    fftwf_iodim dim2= {(int) (numOfPartsPerChannel_ * numOfChannels_), (int) (partSize_), (int) (partSize_) };
    //---
    fftwf_plan fftPlan = fftwf_plan_guru_split_dft(1, &dim, 1, &dim2, real_.data_, imaginary_.data_, real_.data_, imaginary_.data_, FFTW_ESTIMATE);
    //.
    
    //Init Real and Imaginary part (each filter part has half samples from ir and half 0)
    //Real
    for(unsigned int channNum = 0; channNum < numOfChannels_; ++channNum) {
        
        unsigned int partNum;
        
        //parts 0,1... numOfPartsPerChannel_-2 
        for(partNum = 0; partNum < numOfPartsPerChannel_ - 1; ++partNum) {
            memcpy      (real_[channNum] + partNum*/*PART_SIZE*/partSize_, irf->h_[channNum] + partNum*/*BLOCK_SIZE*/blockSize, sizeof(float_type) * blockSize );
            std::fill_n (real_[channNum] + partNum*partSize_ + blockSize,   blockSize, 0);
        }
        //.
        
        //part numOfPartsPerChannel_-1 (last)
        unsigned int numOfSamplesFromIrInLastPart = (irf->N_%blockSize);
        memcpy      (real_[channNum] + partNum*partSize_, irf->h_[channNum] + partNum*blockSize, sizeof(float_type) * numOfSamplesFromIrInLastPart );
        std::fill_n (real_[channNum] + partNum*partSize_ + numOfSamplesFromIrInLastPart,   partSize_ - numOfSamplesFromIrInLastPart, 0);
        //.
        
    }
    //.
    
    //Imaginary
    for(unsigned int sampleNum = 0; sampleNum < dataLength; ++sampleNum)
        imaginary_.data_[sampleNum] = 0;
    //.
    //.
    
    fftwf_execute(fftPlan);
    fftwf_destroy_plan(fftPlan);
}