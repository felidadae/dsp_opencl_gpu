//
//  IRFCsFactory.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 30.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__IRFCsFactory__
#define __FilterModule_Standalone__IRFCsFactory__

#include "IIRFC.h"
#include "FMParamsInfo.h"
//---
#include <iostream>
#include <map>
#include "GeneralExceptions.h"



class IRFCsFactory {
public:
    IRFCsFactory();
    IIRFC* create(FMParamsInfo::irfcIDType id);
    
private:
    typedef IIRFC* (*CreateConvFun)();
    bool RegisterIRFC(FMParamsInfo::irfcIDType id, CreateConvFun fun);
    
    typedef std::map<FMParamsInfo::irfcIDType, CreateConvFun> Callbacks;
    Callbacks callbacks_;
};



#endif /* defined(__FilterModule_Standalone__IRFCsFactory__) */
