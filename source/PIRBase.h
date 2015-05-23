//
//  PIRBase.h
//  DSPOpenCLGPUTester
//
//  Created by kadlubek47 on 07.10.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef DSPOpenCLGPUTester_PIRBase_h
#define DSPOpenCLGPUTester_PIRBase_h

#include "IRF.h"



class PIRBase {
public:
    virtual void    setNewIRF(const IRF* irf, unsigned int blockSize) = 0;
    
    unsigned int    get_numOfPartsPerChannel()      { return numOfPartsPerChannel_; }
    unsigned int    get_partSize()                  { return partSize_; }
    
protected:
    unsigned int numOfPartsPerChannel_  = 0;
    unsigned int partSize_              = 0;
    
    unsigned int numOfChannels_         = 0;
    unsigned int channelLength_         = 0;
};

#endif
