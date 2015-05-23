//
//  FromFile_IRFC.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 01.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef LowPassFilter_Standalone_IRFC_FromFile_h
#define LowPassFilter_Standalone_IRFC_FromFile_h

#include "IRFCBase.h"                       //< Base class
#include "AudioFileIOManager.h"  //< for reading audio file with impulse response



class IRFC_FromFile: public IRFCBase {
public:
    IRFC_FromFile       ();
    void setParameters  (const FMParameters& params);
    
private:
    //Parameters
    std::string irFilePath_;
    //.
    
    void computeImpulseResponse();
    bool checkIfProperAudioFile(std::string nameFile);
    
    AudioFileIOManager<float_type> audioFileIOManager_;
};


#endif
