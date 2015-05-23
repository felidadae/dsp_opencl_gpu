//
//  IRFCBase.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 01.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef LowPassFilter_Standalone_IRFCBase_h
#define LowPassFilter_Standalone_IRFCBase_h

#include "IRF.h"
#include "IIRFC.h"



/*
    IRFC: Impulse Response Function Computator,
    { 0=<i<N h[i] } means the same as irf
*/
class IRFCBase: public IIRFC {
public:
    virtual ~IRFCBase() {}
    const IRF* getPointerToIRF();
    
    void setNotification(bool* irfChanged) { irfChanged_ = irfChanged; }
    void notify() { if(irfChanged_)    *irfChanged_ = true; }
    
protected:
    void deleteIRF();
    
    virtual void computeImpulseResponse() = 0;
    IRF irf_;
    
    bool* irfChanged_ = nullptr;
};



#endif
