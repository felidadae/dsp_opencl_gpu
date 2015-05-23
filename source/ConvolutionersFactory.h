//
//  ConvolutionersFactory.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 06.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __LowPassFilter_Standalone__ConvolutionersFactory__
#define __LowPassFilter_Standalone__ConvolutionersFactory__

#include "IConvolutioner.h"
#include "FMParamsInfo.h"
//---
#include <iostream>
#include <map>
#include "GeneralExceptions.h"



class ConvolutionersFactory {
public:
    ConvolutionersFactory();
    
    IConvolutioner* create(FMParamsInfo::convolutionerIDType id);
    
private:
    typedef IConvolutioner* (*CreateConvFun) ();
    bool RegisterConvolutioner(FMParamsInfo::convolutionerIDType id, CreateConvFun fun);
    
    typedef std::map<FMParamsInfo::convolutionerIDType, CreateConvFun> Callbacks;
    Callbacks callbacks_;
};



#endif /* defined(__LowPassFilter_Standalone__ConvolutionersFactory__) */
