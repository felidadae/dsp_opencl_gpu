//
//  ConvolutionersFactory.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 06.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "ConvolutionersFactory.h"
//---
#include "Convolutioner_TimeDomain.h"
#include "Convolutioner_FrequencyDomain.h"
#include "Convolutioner_FrequencyDomain_OpenCL.h"
//---
#include "FMParamsInfo.h"
#include "IRF.h"



#pragma mark create-convolutioner-functions

IConvolutioner* create_Convolutioner_TimeDomain() {
    return new Convolutioner_TimeDomain();
}

IConvolutioner* create_Convolutioner_FrequencyDomain() {
    return new Convolutioner_FrequencyDomain();
}

IConvolutioner* create_Convolutioner_FrequencyDomain_OpenCL() {
    return new Convolutioner_FrequencyDomain_OpenCL();
}



#pragma mark ConvolutionersFactory implementation

ConvolutionersFactory::ConvolutionersFactory() {
    
    RegisterConvolutioner( FMParamsInfo::NORMAL_CPU__TIME_DOMAIN,                             create_Convolutioner_TimeDomain );
    
    RegisterConvolutioner( FMParamsInfo::NORMAL_CPU__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED,   create_Convolutioner_FrequencyDomain );
    
    RegisterConvolutioner( FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED,       create_Convolutioner_FrequencyDomain_OpenCL );

}

IConvolutioner* ConvolutionersFactory::create (FMParamsInfo::convolutionerIDType id) {
    Callbacks::const_iterator i = callbacks_.find(id);
    if(i == callbacks_.end())
        throw WrongFunArgVal_Exception();
    else
        return (i->second)();
}

bool ConvolutionersFactory::RegisterConvolutioner   (FMParamsInfo::convolutionerIDType id, CreateConvFun fun) {
    Callbacks::const_iterator i = callbacks_.find(id);
    if(i == callbacks_.end()) {
        callbacks_.insert(std::pair<FMParamsInfo::convolutionerIDType, CreateConvFun>(id, fun));
        return true;
    }
    else
        return false;
}




