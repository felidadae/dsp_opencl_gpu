//
//  AudioInOutBuffers.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 03.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef LowPassFilter_Standalone_AudioBlocks_h
#define LowPassFilter_Standalone_AudioBlocks_h

#include "FMTypes.h"
#include "AudioBuffer.h"


template<class SampleType>
class AudioInOutBuffers{
public:
    AudioBuffer<SampleType> in_;
    AudioBuffer<SampleType> out_;
    unsigned int numOfChannels_;
    unsigned int channelLength_;
    
    AudioInOutBuffers(SampleType* inputBlocks, SampleType* outputBlocks, unsigned int numOfChannels, unsigned int blockSize): in_(numOfChannels_, channelLength_), out_(numOfChannels_, channelLength_) {
        in_             = inputBlocks;
        out_            = outputBlocks;
        numOfChannels_  = numOfChannels;
        channelLength_  = blockSize;
    }
    AudioInOutBuffers():    in_(numOfChannels_, channelLength_), out_(numOfChannels_, channelLength_) {}
};

#endif
