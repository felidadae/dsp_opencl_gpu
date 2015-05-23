//
//  FilterModule.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 01.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __LowPassFilter_Standalone__FilterModule__
#define __LowPassFilter_Standalone__FilterModule__

#include "IProcess.h"
#include "IParameterizable.h"

#include "FMTypes.h"  //<Samples data type

#include "IRFCsManager.h"
#include "ConvolutionersManager.h"



class FilterModule: public IParameterizable<FMParameters>, public IProcess<float_type> {
public:
    
    //Construct, destroy
     FilterModule();
    ~FilterModule();
    //.
    
    //IParameterizable
    void setParameters  (const FMParameters& params);
    //.
    
    //IProcess
    void process        (AudioInOutBuffers<float_type>& audioBlocks);
    void reset();
    //.
    
    //ir info - impulse response length and number of channels
    unsigned int getIRLength();
    unsigned int getIRNumOfChann();
    //.
    
private:
    
    IRFCsManager          irfcManager_;
    ConvolutionersManager convolutionersManager_;
    
    bool irfChanged_ = false;   //< notification
};


#endif /* defined(ProcessBase) */
