//
//  ConvolutionersManager.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 06.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef LowPassFilter_Standalone_ConvolutionersManager_h
#define LowPassFilter_Standalone_ConvolutionersManager_h

#include "IConvolutioner.h"
#include "IParameterizable.h"
//---
#include "FMParamsInfo.h"
//---
#include "IConvolutioner.h"
#include "ConvolutionersFactory.h"



class ConvolutionersManager: public IConvolutioner, public IParameterizable<FMParameters> {
public:
    ConvolutionersManager ();
    ~ConvolutionersManager ();
    
    void setParameters (const FMParameters& params);
    
    void setNewIR (const IRF* irf);
    
    void process (AudioInOutBuffers<float_type>& audio );
    void reset ();
    
private:
    FMParamsInfo::convolutionerIDType convolutionerID_;
    IConvolutioner* convolutioner_ = nullptr;
    
    ConvolutionersFactory factory_;
    
    const IRF* irf_;
};



#endif
