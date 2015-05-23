//
//  Convolutioner_FrequencyDomain.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 06.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "Convolutioner_FrequencyDomain.h"
#include <cmath>
#include <algorithm> 
#include <iostream>
#include "DebugFunctions.h"




void complexMultiplyAdd (unsigned int problemSize                     /*2B * in_ChannelsWindow_numOfChannels */,
                         float*  fdl_R,  float*  fdl_I,               /*IN  delay lines in frequency domain */
                         float* out_cw_R,  float* out_cw_I,           /*OUT ganged channels windows being result of complex multiplying and adding, in frequency domain */
                         float*    pir_R,  float*    pir_I,           /*pir == partitioned impulse response, ganged (one pir channel after another) in frequency domain */
                         const unsigned int _2B,                      /*part size, length of one channel window*/
                         const unsigned int _C,                       /*number of channels in fdl*/
                         const unsigned int _P,                       /*number of ir parts  >>>>>per one channel<<<<< */
                         const unsigned int pir_C,                    /*number of channels of ir*/
                         const unsigned int fdl_cursor_current        /*cursor at the newest fdl line (that which has been just inserted)*/
                         );



#pragma mark Creation

Convolutioner_FrequencyDomain::Convolutioner_FrequencyDomain() {}

Convolutioner_FrequencyDomain::~Convolutioner_FrequencyDomain() {
    float_type** buffers[4] = { &bufferTransform_R_, &bufferTransform_I_, &bufferAccumulator_R_, &bufferAccumulator_I_  };
    if(bufferTransform_R_)
        for(unsigned int iBuffer = 0; iBuffer < 4; ++iBuffer)
            fftwf_free( *(buffers[iBuffer]) );
}



#pragma mark IConvoluter interface implementation

void Convolutioner_FrequencyDomain::setNewIR(const IRF* irf) {
    irf_ = irf;
    if(window_.get_inputBlockSize() != 0)
        partitionedIR_.setNewIRF( irf_, window_.get_channelLength() );
}

void Convolutioner_FrequencyDomain::process(AudioInOutBuffers<float_type>& audio ) {
    
    //
    unsigned int _2B = audio.channelLength_ * 2;
    unsigned int _B  = audio.channelLength_;
    unsigned int _C  = audio.numOfChannels_;                        //numOfChannels
    unsigned int _P  = partitionedIR_.get_numOfPartsPerChannel();   //numOfIRPartsPerChannel
    //.
    
    //_ if latency or number of channels changed:
    //      set partitionedIR
    //      recreate buffers
    //      recreate fft plans
    if ( window_.get_inputBlockSize() != audio.channelLength_ || window_.get_numOfChannels() != audio.numOfChannels_) {
        
        //Setting partitionedIR
        if (window_.get_inputBlockSize() != audio.channelLength_) {
            partitionedIR_.setNewIRF( irf_, audio.channelLength_ );
            _P = partitionedIR_.get_numOfPartsPerChannel();
        }
        //.
        
        //Recreate buffers
        //delete
        float_type** buffers[4] = { &bufferTransform_R_, &bufferTransform_I_, &bufferAccumulator_R_, &bufferAccumulator_I_  };
        if(bufferTransform_R_) {
            for(unsigned int iBuffer = 0; iBuffer < 4; ++iBuffer)
                fftwf_free( *(buffers[iBuffer]) );
            fftwf_free(bufferFDL_R_);
            fftwf_free(bufferFDL_I_);
        }
        //.
        
        //Create
        for(unsigned int iBuffer = 0; iBuffer < 4; ++iBuffer)
            *(buffers[iBuffer])   = (float_type*) fftwf_malloc(sizeof(float_type) * /*>>>*/_2B * _C/*<<<*/);
        bufferFDL_R_= (float_type*) fftwf_malloc(sizeof(float_type) * _2B * _C * _P );
        bufferFDL_I_= (float_type*) fftwf_malloc(sizeof(float_type) * _2B * _C * _P );
        //.
        
        //initialize FDL with 0
        std::fill_n (bufferFDL_R_, _2B * _C * _P, 0);
        std::fill_n (bufferFDL_I_, _2B * _C * _P, 0);
        lastInsertedDelayLineIdx = 0;
        //.
        //.(Recreate buffers)
        
        //Recreate plans
        fftwf_iodim dim      =  { (int)(_2B),                         1,          1   };
        fftwf_iodim dimArray =  { (int)(audio.numOfChannels_), (int)(_2B), (int)(_2B) };
        //--
        fftPlan_Forward_ = fftwf_plan_guru_split_dft(1, &dim, 1, &dimArray, bufferTransform_R_,  bufferTransform_I_,  bufferTransform_R_,  bufferTransform_I_,  FFTW_ESTIMATE);
        fftPlan_Backward_= fftwf_plan_guru_split_dft(1, &dim, 1, &dimArray, bufferAccumulator_I_, bufferAccumulator_R_, bufferAccumulator_I_, bufferAccumulator_R_, FFTW_ESTIMATE);
        //.
    }
    
    //Update channelsWindow
    window_.update( audio, /*history size*/ _B );
    //.
    
    //Init >>bufferTransform<<
    memcpy      (bufferTransform_R_, window_.buffer_, window_.buffer_.get_allLength() * sizeof(float));
    std::fill_n (bufferTransform_I_, window_.buffer_.get_allLength(), 0);
    //.
    
    //Make fft of new delayLine
    fftwf_execute(fftPlan_Forward_);
    //.
    
    //Copy new fdl line from bufferTransform into bufferFDL
    memcpy(bufferFDL_R_ + lastInsertedDelayLineIdx * (_2B * _C ), bufferTransform_R_, _2B * _C * sizeof(float));
    memcpy(bufferFDL_I_ + lastInsertedDelayLineIdx * (_2B * _C ), bufferTransform_I_, _2B * _C * sizeof(float));
    //.
    
    //increment lastInsertedDelayLine
    unsigned int currDelayLineIdx = lastInsertedDelayLineIdx;
    lastInsertedDelayLineIdx = (lastInsertedDelayLineIdx + 1 ) % _P;
    //.
    //.

    //Execute kernel
    complexMultiplyAdd  (   _2B * _C,
                            bufferFDL_R_, bufferFDL_I_,
                            bufferAccumulator_R_, bufferAccumulator_I_,
                            partitionedIR_.real_, partitionedIR_.imaginary_,
                            _2B,
                            _C,
                            _P,
                            irf_->numOfChannels_,
                            currDelayLineIdx);
    //.
    
    //ifft
    fftwf_execute(fftPlan_Backward_);
    //.
    
    
    
    //write fftw vector form to audio.outputChannel[number of Channel]
    for (unsigned int channNum = 0; channNum < _C; ++channNum)
        for (unsigned sampleNum = 0; sampleNum < _B; ++sampleNum)
            audio.out_[channNum][sampleNum] = (bufferAccumulator_R_[channNum*_2B + _B + sampleNum])/(_2B);
    //.
}



#pragma mark CompleMultiplyAdd

void complexMultiplyAdd (unsigned int problemSize                     /* 2B * C */,
                         float*  fdl_R,  float*  fdl_I,               /*IN  delay lines in frequency domain */
                         float* out_cw_R,  float* out_cw_I,           /*OUT ganged channels windows being result of complex multiplying and adding, in frequency domain */
                         float*    pir_R,  float*    pir_I,           /*pir == partitioned impulse response, ganged (one pir channel after another) in frequency domain */
                         const unsigned int _2B,                      /*part size, length of one channel window*/
                         const unsigned int _C,                       /*number of channels in fdl*/
                         const unsigned int _P,                       /*number of ir parts  >>>>>per one channel<<<<< */
                         const unsigned int pir_C,                    /*number of channels of ir*/
                         const unsigned int FDL_LINE                  /*cursor at the newest fdl line (that which has been just inserted)*/
                         )
{
    
    for(unsigned int idx = 0; idx < problemSize; ++idx) {
        
        
        //channel num
        //As if [channNum][sampleNum]
        unsigned int channNum = idx / _2B;
        unsigned int sampleNum= idx % _2B;
        //.
        
        //pir index
        unsigned int pir_Idx        = sampleNum % (pir_C * _2B);    //in case if there is no the same number of channels of input and ir
        //.
        
        //fdl index
        // [[FDL(chann=0, line=0)] [FDL(chann=1, line=0)] ... [FDL(chann=N-1, line=0)]]  [ {FDL(chann={0:N-1}, line = 1)} ] ... [FDL(chann={0:N-1}, line = numOfPartsIRPerChannel)]
        unsigned int fdl_Idx_base   = channNum * _2B +  sampleNum ;
        unsigned int fdl_cursor     = FDL_LINE;
        unsigned int fdl_Idx        = (fdl_cursor * _2B * _C) + fdl_Idx_base;
        //.
        
        
        
        //local accumulator
        float out_cw_R_local = 0,            out_cw_I_local = 0;
        //.
        
        //complex multiply add
        for(unsigned int partNum = 0; partNum < _P; ++partNum) {
            
            //copy to local memory
            float pir_R_local = pir_R[pir_Idx];
            float pir_I_local = pir_I[pir_Idx];
            //---
            float fdl_R_local = fdl_R[fdl_Idx];
            float fdl_I_local = fdl_I[fdl_Idx];
            //.
            
            //complex multiply
            //(a, b)*(c,d)= (ac - bd, bc + ad)
            out_cw_R_local += fdl_R_local * pir_R_local - fdl_I_local * pir_I_local; 
            out_cw_I_local += fdl_I_local * pir_R_local + fdl_R_local * pir_I_local;
            //.
            
            //increment indexes
            pir_Idx += _2B * _C; 
            fdl_cursor  = (fdl_cursor - 1 + _P) % _P;
            fdl_Idx  = (fdl_cursor * _2B * _C) + fdl_Idx_base;
            //.
            
        }//.
        
        //writing result to global memory
        out_cw_R[idx] = out_cw_R_local;
        out_cw_I[idx] = out_cw_I_local;
        //.
    }
    
}