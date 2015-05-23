//
//  Convolutioner_TimeDomain.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 04.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "Convolutioner_TimeDomain.h"



void Convolutioner_TimeDomain::process(AudioInOutBuffers<float_type>& audio) {
    //Update channelsWindow
    window_.update(audio, 2 * irf_->M_);
    //.
    
    //Pre loop
    float_type* x = window_[0];
    float_type* y = audio.out_[0];
    int channNum = 0;
    unsigned int irf_channNum = 0;
    //.
    
    //Lopp
    while (channNum < audio.numOfChannels_) {
        
        for(unsigned int index_y = 0; index_y < audio.channelLength_; ++index_y) {
            
            y[index_y] = 0;
            unsigned int index_h = 0;
            unsigned int index_x = index_y;
            
            for (; index_h < irf_->N_; ++index_h, ++index_x)
                y[index_y] += x[index_x] * irf_->h_[ /*loop impulse responses channels through input channels*/  irf_channNum][index_h];
        }
        
        //Loop Increment
        ++channNum;
        x = window_[channNum];
        y = audio.out_[channNum];
        irf_channNum = channNum%irf_->numOfChannels_;
        //.
        
    }
    //.
}
