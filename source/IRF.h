//
//  IRF.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 03.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef LowPassFilter_Standalone_IRF_h
#define LowPassFilter_Standalone_IRF_h

#include "FMTypes.h"
#include "AudioBuffer.h"



class IRF {
public:
    IRF(): h_(N_, numOfChannels_) {}
    
    AudioBuffer<float_type> h_;
    unsigned int numOfChannels_ = 0;
    unsigned int N_ = 0, M_ = 0;
};



#endif
