//
//  Convolutioner_TimeDomain.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 04.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __LowPassFilter_Standalone__Convolutioner_TimeDomain__
#define __LowPassFilter_Standalone__Convolutioner_TimeDomain__

#include "ConvolutionerBase.h"
#include "ChannelsWindow.h"



class Convolutioner_TimeDomain: public ConvolutionerBase {
public:
    Convolutioner_TimeDomain() {}
    
    void setNewIR(const IRF* irf) { irf_ = irf;}
    
    void process(AudioInOutBuffers<float_type>& audio);
};



#endif /* defined(__LowPassFilter_Standalone__Convolutioner_TimeDomain__) */
