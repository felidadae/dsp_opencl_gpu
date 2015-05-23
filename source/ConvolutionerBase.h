//
//  ConvolutionerBase.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 26.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__ConvolutionerBase__
#define __FilterModule_Standalone__ConvolutionerBase__

#include "IConvolutioner.h"
#include "ChannelsWindow.h"



class ConvolutionerBase: public IConvolutioner {
public:
    void reset() {window_.reset();}
    
protected:
    ChannelsWindow window_;
    const IRF* irf_ = nullptr;
};



#endif /* defined(__FilterModule_Standalone__ConvolutionerBase__) */
