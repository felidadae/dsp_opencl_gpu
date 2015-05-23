//
//  IProcess.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 26.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef FilterModule_Standalone_IProcess_h
#define FilterModule_Standalone_IProcess_h

#include "AudioInOutBuffers.h"


template<class SampleType>
class IProcess {
public:
    virtual ~IProcess() {}
    virtual void process (AudioInOutBuffers<SampleType>& audioBlocks) = 0;
    virtual void reset() {};
};


#endif
