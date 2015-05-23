//
//  IIRFC.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 30.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef FilterModule_Standalone_IIRFC_h
#define FilterModule_Standalone_IIRFC_h

#include "IParameterizable.h"
#include "FMParamsInfo.h"
#include "IRF.h"



class IIRFC: public IParameterizable<FMParameters> {
public:
    virtual ~IIRFC() {}
    virtual const IRF* getPointerToIRF() = 0;
    
    virtual void setNotification(bool* irfChanged) = 0;
    virtual void notify() = 0;
};



#endif
