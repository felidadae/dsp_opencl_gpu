//
//  IRFCsFactory.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 30.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//
#include "IfTesting.h"
//---
#include "IRFCsFactory.h"
//---
#include "IRFC_LowPass.h"
#include "IRFC_FromFile.h"
#ifdef FILTER_MODULE__ADD_TESTING
#include "IRFC_Testing.h"
#endif



#pragma mark create-irfc-functions

IIRFC* create_irfc_LowPass()  {
    return new IRFC_LowPass();
}

IIRFC* create_irfc_fromFile() {
    return new IRFC_FromFile();
}

#ifdef FILTER_MODULE__ADD_TESTING
IIRFC* create_irfc_testing() {
    return new IRFC_Testing();
}
#endif



#pragma mark IRFCsFactory implementation

IRFCsFactory::IRFCsFactory() {
    RegisterIRFC( FMParamsInfo::LOW_PASS, create_irfc_LowPass  );
    RegisterIRFC( FMParamsInfo::FROM_FILE, create_irfc_fromFile );
#ifdef FILTER_MODULE__ADD_TESTING
    RegisterIRFC( FMParamsInfo::TEST, create_irfc_testing);
#endif
}

IIRFC* IRFCsFactory::create ( FMParamsInfo::irfcIDType id ) {
    Callbacks::const_iterator i = callbacks_.find(id);
    if(i == callbacks_.end())
        throw WrongFunArgVal_Exception();
    else
        return (i->second)();
}

bool IRFCsFactory::RegisterIRFC ( FMParamsInfo::irfcIDType id, CreateConvFun fun) {
    Callbacks::const_iterator i = callbacks_.find(id);
    if(i == callbacks_.end()) {
        callbacks_.insert(std::pair<FMParamsInfo::irfcIDType, CreateConvFun>(id, fun));
        return true;
    }
    else
        return false;
}




