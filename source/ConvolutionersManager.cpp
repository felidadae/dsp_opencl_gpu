//
//  ConvolutionersManager.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 06.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "ConvolutionersManager.h"
#include "FMParamsInfo.h"



ConvolutionersManager::ConvolutionersManager() {}
ConvolutionersManager::~ConvolutionersManager () { if(convolutioner_) delete convolutioner_; }

void ConvolutionersManager::setParameters (const FMParameters& params) {
    
    FMParamsInfo::convolutionerIDType convolutionerID = convolutionerID_;
    
    //if there is no parameter convolutionerID
    if (!params.getValue(FMParamsInfo::convolutionerID, convolutionerID))
        return; //do nothing
    //.
    
    if(!convolutioner_ || (convolutioner_ && convolutionerID != convolutionerID_)) {
        if(convolutioner_) delete convolutioner_;
        convolutioner_ = factory_.create(convolutionerID);
        convolutionerID_ = convolutionerID;
    }
    
}

void ConvolutionersManager::setNewIR(const IRF* irf) {
    irf_ = irf;
    if(convolutioner_)
        convolutioner_->setNewIR(irf_);
}



void ConvolutionersManager::process (AudioInOutBuffers<float_type>& audio) {
    if( !convolutioner_ || !irf_->h_.data_ )
        return;
    convolutioner_->process(audio);
}

void ConvolutionersManager::reset() {
    if(convolutioner_)
        convolutioner_->reset();
}