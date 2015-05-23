//
//  IRFCsManager.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 30.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__IRFCsManager__
#define __FilterModule_Standalone__IRFCsManager__

#include "IParameterizable.h"
#include "FMParamsInfo.h"
//---
#include "IIRFC.h"
#include "IRFCsFactory.h"



class IRFCsManager: public IParameterizable<FMParameters> {
public:
    IRFCsManager(bool* irfChanged) {
        irfChanged_ = irfChanged;
    }
    ~IRFCsManager() {
        if(irfc_) delete irfc_;
    }
    
    void setParameters(const FMParameters& params);
    
    const IRF* getPointerToIRF();
    
private:
    FMParamsInfo::irfcIDType irfcID_;
    IIRFC* irfc_        = nullptr;
    bool* irfChanged_   = nullptr;  //notification
    
    IRFCsFactory factory_;
};


#endif /* defined(__FilterModule_Standalone__IRFCsManager__) */
