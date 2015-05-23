//
//  Convolutioner_FrequencyDomain.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 06.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __LowPassFilter_Standalone__Convolutioner_FrequencyDomain__
#define __LowPassFilter_Standalone__Convolutioner_FrequencyDomain__

#include "ConvolutionerBase.h"
#include "PIR_fftw.h"
//---
#include "fftw3/fftw3.h"



class Convolutioner_FrequencyDomain: public ConvolutionerBase {
public:
     Convolutioner_FrequencyDomain();
    ~Convolutioner_FrequencyDomain();
    
    void setNewIR(const IRF* irf);
    
    void process(AudioInOutBuffers<float_type>& audio );
    
private:
    PIR_fftw partitionedIR_;
    
    //FDL cursor (cursor where is delay line which was been inserted the earliear and now will be overwritten)
    unsigned int lastInsertedDelayLineIdx = 0;
    //.
    
    //fft plans
    fftwf_plan fftPlan_Forward_, fftPlan_Backward_;
    //.
    
    //Buffers
    float_type* bufferTransform_R_ = nullptr,  *bufferTransform_I_ = nullptr, * bufferAccumulator_R_= nullptr, *bufferAccumulator_I_ = nullptr; //< [channel0][channel1]...[channelN]
    //---
    float_type* bufferFDL_R_ = nullptr, *bufferFDL_I_ = nullptr; // [[FDL(chann=0, line=0)] [FDL(chann=1, line=0)] ... [FDL(chann=N-1, line=0)]]  [ {FDL(chann={0:N-1}, line = 1)} ] ... [FDL(chann={0:N-1}, line = numOfPartsIRPerChannel)]
    //.
};



#endif /* defined(__LowPassFilter_Standalone__Convolutioner_FrequencyDomain__) */
