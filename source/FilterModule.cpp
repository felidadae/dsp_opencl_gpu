//
//  FilterModule.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 05.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "FilterModule.h"



#pragma mark Creating, destroying

FilterModule::FilterModule(): irfcManager_(&irfChanged_) {}

FilterModule::~FilterModule() {}



#pragma mark IParameterizable

void FilterModule::setParameters(const FMParameters& params) {
    /********/irfcManager_.setParameters(params);
    convolutionersManager_.setParameters(params);
    
    if(irfChanged_) {
        convolutionersManager_.setNewIR(irfcManager_.getPointerToIRF());
        irfChanged_ = false;
    }
}



#pragma mark IProcess

void FilterModule::process(AudioInOutBuffers<float_type>& audioBlocks) {
    convolutionersManager_.process(audioBlocks);
}

void FilterModule::reset() { convolutionersManager_.reset(); }



#pragma mark other

unsigned int FilterModule::getIRLength() {
    if(irfcManager_.getPointerToIRF())
        return irfcManager_.getPointerToIRF()->N_;
    return 0;
}

unsigned int FilterModule::getIRNumOfChann() {
    if(irfcManager_.getPointerToIRF())
        return irfcManager_.getPointerToIRF()->numOfChannels_;
    return 0;
}






