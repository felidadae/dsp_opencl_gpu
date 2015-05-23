//
//  IRFC_Testing.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 04.09.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//
#include "IfTesting.h"
//---
#include "IRFC_Testing.h"



#ifdef FILTER_MODULE__ADD_TESTING //only in testing version

void IRFC_Testing::setParameters  (const FMParameters& params) {
    unsigned int  N     = irf_.N_;
    unsigned int ir_C   = irf_.numOfChannels_;
    params.getValue(FMParamsInfo::length, N);
    params.getValue(FMParamsInfo::numOfChannels, ir_C);
    
    if(N != irf_.N_ || ir_C != irf_.numOfChannels_) {
        irf_.N_ = N;
        irf_.numOfChannels_ = ir_C;
        
        deleteIRF();
        computeImpulseResponse();
        notify();
    }
    
}

void IRFC_Testing::computeImpulseResponse() {
    unsigned int newLength = irf_.N_ * irf_.numOfChannels_;
    irf_.h_ = new float_type[newLength];
    srand((unsigned int)(time(NULL)));
    for(unsigned int i = 0; i < newLength; ++i)
        irf_.h_.data_[i] = ((float_type)(rand()%300))/300.0f;
}

#endif //TESTING