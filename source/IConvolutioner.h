//
//  ConvolutionerBase.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 03.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __LowPassFilter_Standalone__ConvolutionerBase__
#define __LowPassFilter_Standalone__ConvolutionerBase__

#include "IProcess.h"
#include "FMTypes.h"
#include "IRF.h"
#include "AudioInOutBuffers.h"



class IConvolutioner: public IProcess<float_type> {
public:
    virtual ~IConvolutioner() {}
    
    virtual void setNewIR(const IRF* irf) = 0;
};



#endif /* defined(__LowPassFilter_Standalone__ConvolutionerBase__) */















































//------------------------------------------------------------------------------------------------


/*
 optimal when 2*M < inputBufferSize
 */
/*
class SignalWindow
{
public:
    float_type* signal_ = NULL;
    unsigned int signalLength_ = 0;
    
    SignalWindow();
    
    ~SignalWindow() {delete [] signal_;}
    
    void setParameter(unsigned int M, unsigned int x_size){
        if( x_size==0)
            return;
        
        if( M + x_size != M_+x_size_){
            delete [] signal_;
            
            signalLength_ = 2*M + x_size;
            signal_ = new float_type[signalLength_]();
        }
        
        M_ = M;
        x_size_ = x_size;
    }
    
    void update(float_type* x) {
        unsigned int i;
        for(i=0; i < 2*M_; ++i)
            signal_[i] = signal_[signalLength_-2*M_+i];
        for(int j=0; j < x_size_; ++j, ++i)
            signal_[i] = x[j];
    }
    
    void clear() {
        for(int i=0; i < signalLength_; ++i)
            signal_[i] = 0;
    }
    
private:
    unsigned int M_ = 0, x_size_ = 0;
};








class ChannelProcessingUnit
{
public:
    ChannelProcessingUnit();
    
    void process(float_type* inputBlock, int inputBlockSize, float_type* outputBlock);
    
    void reset();
    
private:

    SignalWindow signal_;
    void convoluteWithImpulseResponse (float_type* x, int sizeX, float_type* y);
    
};*/