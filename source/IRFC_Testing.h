//
//  IRFC_Testing.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 04.09.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__IRFC_Testing__
#define __FilterModule_Standalone__IRFC_Testing__
#include "IfTesting.h"
//---
#ifdef FILTER_MODULE__ADD_TESTING   //only in testing version
#include "IRFCBase.h"               //< Base class



class IRFC_Testing: public IRFCBase {
public:
    void setParameters  (const FMParameters& params);
private:
    void computeImpulseResponse();
};

#endif


//---
#endif /* defined(__FilterModule_Standalone__IRFC_Testing__) */
