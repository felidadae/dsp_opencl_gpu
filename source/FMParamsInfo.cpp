//
//  FMParamsInfo.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 31.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "FMParamsInfo.h"



const FMParamsInfo::irfcIDType                  FMParamsDefVal::default_irfcID             =   FMParamsInfo::FROM_FILE;
const FMParamsInfo::convolutionerIDType         FMParamsDefVal::default_convolutionerID    =   FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED;
const unsigned int                              FMParamsDefVal::default_samplingRate       =   44100;

const float_type                                FMParamsDefVal::default_oscillationInPass  =   0.001;
const float_type                                FMParamsDefVal::default_oscillationInStop  =   0.001;
const unsigned int                              FMParamsDefVal::default_fpass              =   600;
const unsigned int                              FMParamsDefVal::default_fstop              =   610;

std::string                                     FMParamsDefVal::default_irFilePath         =   std::string("reverb_samples/HyperVerb-StateCapitol.wav");



void FMParamsDefVal::setDefault (FMParameters& param) {
    param.add(FMParamsInfo::irfcID,             FMParamsDefVal::default_irfcID              );
    param.add(FMParamsInfo::convolutionerID,    FMParamsDefVal::default_convolutionerID     );
    
    param.add(FMParamsInfo::oscillationInPass,  FMParamsDefVal::default_oscillationInPass   );
    param.add(FMParamsInfo::oscillationInStop,  FMParamsDefVal::default_oscillationInStop   );
    param.add(FMParamsInfo::fpass,              FMParamsDefVal::default_fpass               );
    param.add(FMParamsInfo::fstop,              FMParamsDefVal::default_fstop               );
    param.add(FMParamsInfo::samplingRate,       FMParamsDefVal::default_samplingRate        );
    
    param.add(FMParamsInfo::irFilePath,         FMParamsDefVal::default_irFilePath          );
}


