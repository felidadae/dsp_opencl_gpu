//
//  IRFC_FromFile.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 02.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "IRFC_FromFile.h"



#pragma mark Construct, destroy

IRFC_FromFile::IRFC_FromFile() {}

void IRFC_FromFile::setParameters(const FMParameters& params) {
    
    std::string  irFilePath = irFilePath_;
    params.getValue(FMParamsInfo::irFilePath, irFilePath);
    
    if(irFilePath != irFilePath_ && checkIfProperAudioFile(irFilePath)) {
        irFilePath_ = irFilePath;
        deleteIRF();
        computeImpulseResponse();
        notify();
    }
}




void IRFC_FromFile::computeImpulseResponse() {
    
    //Read ir from file
    {
        AudioFileIOManager<float_type>::InStruct str;
        audioFileIOManager_.read ( &irf_.h_.data_, irFilePath_.c_str(), str );
        irf_.N_ = (unsigned int) str.channelLength;
        irf_.numOfChannels_ = str.numOfChannels;
    }
    //.
    
    //@TODO sample rate should be changed accordingly to input channels 
    
    //
    if(irf_.N_%2 == 0 )
        irf_.N_ -=1;
    irf_.M_ = (irf_.N_-1)/2;
    //.
    
    //normalize ir - (first channel sum)
    float_type sum = 0;
    for(int i = 0; i < irf_.N_; ++i)
        sum += irf_.h_[0][i];
    sum < 0 ? sum = -sum : 1;
    for(unsigned int channNum = 0; channNum < irf_.numOfChannels_; ++channNum )
        for(unsigned int sampleNum = 0; sampleNum < irf_.N_; ++sampleNum)
            irf_.h_[channNum][sampleNum] /= sum;
    //.
}

bool IRFC_FromFile::checkIfProperAudioFile(std::string nameFile) {
    //@TODO make proper check
    
    if(nameFile.empty())
        return false;
    return true;
}

