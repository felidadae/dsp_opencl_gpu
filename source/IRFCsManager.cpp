//
//  IRFCsManager.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 30.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "IRFCsManager.h"



void IRFCsManager::setParameters(const FMParameters& params) {
    
    FMParamsInfo::irfcIDType irfcID = irfcID_;
    
    //if there is no parameter irfcID
    if (!params.getValue(FMParamsInfo::irfcID, irfcID))
        return; //do nothing
    //.
    
    if(!irfc_ || (irfc_ && irfcID != irfcID_)) {
        if(irfc_) delete irfc_;
        irfc_ = factory_.create(irfcID);
        irfc_->setNotification(irfChanged_);
        irfc_->setParameters(params);
        irfcID_ = irfcID;
    }
}

const IRF* IRFCsManager::getPointerToIRF() {
    if(irfc_)
        return irfc_->getPointerToIRF();
    return nullptr;
}

