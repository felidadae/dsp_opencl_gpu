//
//  FMParamsInfo.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 31.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__FMParametersInfo__
#define __FilterModule_Standalone__FMParametersInfo__
#include "IfTesting.h"
//--------------------

#include "FMTypes.h"
#include "IParameterizable.h"
#include <string>
#include "language_pl.h"



class FMParamsInfo {
public:
    
    enum Name {
        
        //General
        irfcID,             //irfcIDType
        convolutionerID,    //convolutionerIDType
        samplingRate,       //unsigned int
        //.
        
        //IRFC_LowPass
        oscillationInPass,  //float_type
        oscillationInStop,  //float_type
        fpass,              //unsigned int
        fstop,              //unsigned int
        //.
        
        //IRFC_FromFile
        irFilePath,         //std::string
        //.
        
        #ifdef FILTER_MODULE__ADD_TESTING
        //IRFC_Testing
        length,             //unsigned int
        numOfChannels       //unsigned int
        //.
        #endif
        
    };
    
    enum irfcIDType {
        LOW_PASS,
        FROM_FILE
        #ifdef FILTER_MODULE__ADD_TESTING
        ,TEST
        #endif
    };
    
    enum convolutionerIDType {
        NORMAL_CPU__TIME_DOMAIN,
        
        NORMAL_CPU__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED,
        /**/OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED
    };
    
    
    
    static std::string getString(irfcIDType id ) {
        switch (id) {
            case LOW_PASS:
                return std::string("irfc low pass");
                
            case FROM_FILE:
                return std::string("irfc from file");
                
#ifdef FILTER_MODULE__ADD_TESTING
            case TEST:
                return std::string("irfc testing");
#endif
        }
    }
    static std::string getString(convolutionerIDType id) {
        switch (id) {
            case NORMAL_CPU__TIME_DOMAIN:
                return std::string(PROPERTIE_VALUE__CONVOLUTIONER__CPU__DEFINITION_TIME_DOMAIN);
            case NORMAL_CPU__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED:
                return std::string(PROPERTIE_VALUE__CONVOLUTIONER__CPU__UNIFORMLY_PARTITIONED);
            case OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED:
                return std::string(PROPERTIE_VALUE__CONVOLUTIONER__OPENCL_GPU__UNIFORMLY_PARTITIONED);
        }
    }
};



typedef Parameters<FMParamsInfo::Name> FMParameters;



class FMParamsDefVal {
public:
    //General
    static const FMParamsInfo::irfcIDType             default_irfcID;
    static const FMParamsInfo::convolutionerIDType    default_convolutionerID;
    static const unsigned int                         default_samplingRate;
    //.
    
    //irfcLowPass
    static const float_type                           default_oscillationInPass;
    static const float_type                           default_oscillationInStop;
    static const unsigned int                         default_fpass;
    static const unsigned int                         default_fstop;
    //.
    
    //irfcFromFile
    static std::string                                default_irFilePath;
    //.
    
    
    //_________________________________________
    static void setDefault (FMParameters& param);
};



#endif /* defined(__FilterModule_Standalone__FMParametersInfo__) */
