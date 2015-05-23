//
//  PartitionedIR_freqDomain.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 27.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__PartitionedIRProvider__
#define __FilterModule_Standalone__PartitionedIRProvider__

#include <cmath>
#include "fftw3/fftw3.h"
#include "IRF.h"
#include "AudioBuffer.h"



class PartitionedIR_freqDomain {
public:
    PartitionedIR_freqDomain(): imaginary_(numOfChannels_, channelLength_), real_(numOfChannels_, channelLength_) {}
    
    void            setNewIRF(const IRF* irf, unsigned int blockSize);
    
    unsigned int    get_numOfPartsPerChannel()      { return numOfPartsPerChannel_; }
    unsigned int    get_partSize()                  { return partSize_; }

    //
    AudioBuffer<float_type> imaginary_;
    AudioBuffer<float_type> real_;
private:
    unsigned int numOfChannels_         = 0;
    unsigned int channelLength_         = 0;
    //.
    
    unsigned int numOfPartsPerChannel_  = 0;
    unsigned int partSize_              = 0;
};



class FFTs_PartitionIR_Provider_OpenCL {
public:
    
    
};



#endif /* defined(__FilterModule_Standalone__PartitionedIRProvider__) */
