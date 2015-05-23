//
//  IRFC_LowPass.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 01.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include <cmath>    //< for log, M_PI, ceil
//---
#include "IRFC_LowPass.h"
#include "FMParamsInfo.h"



#pragma mark Constructing, destroying, setting parameters

IRFC_LowPass::IRFC_LowPass() {
    prepareBesselAprox();
}

void IRFC_LowPass::setParameters(const FMParameters& params) {
    
    float_type oscillationInPass    = oscillationInPass_;
	float_type oscillationInStop    = oscillationInStop_;
    unsigned int fpass              = fpass_;
    unsigned int fstop              = fstop_;
	unsigned int  samplingRate      = samplingRate_;
    
    params.getValue( FMParamsInfo::oscillationInPass,     oscillationInPass   );
    params.getValue( FMParamsInfo::oscillationInStop,     oscillationInStop   );
    params.getValue( FMParamsInfo::fpass,                 fpass               );
    params.getValue( FMParamsInfo::fstop,                 fstop               );
    params.getValue( FMParamsInfo::samplingRate,          samplingRate        );
    
    if(oscillationInPass != oscillationInPass_ || oscillationInStop != oscillationInStop_ || fpass != fpass_ || fstop != fstop_ || samplingRate != samplingRate_) {
        oscillationInPass_  = oscillationInPass;
        oscillationInStop_  = oscillationInStop;
        fpass_              = fpass;
        fstop_              = fstop;
        samplingRate_       = samplingRate;
        
        deleteIRF();
        computeImpulseResponse();
        
        //irf_.h_ = irf_.h_+irf_.M_;
        //irf_.N_ /= 2;
        
        notify();
    }
}



#pragma mark Mathematical computation

void IRFC_LowPass::computeImpulseResponse() {
    //1. Assignments
    //1.1. ---
        
    //1.2. normalizedPulsation, normalizedFrequency;
    normalizedFrequency_=((float_type)(fpass_+fstop_))/2/samplingRate_;
    normalizedPulsation_=2*((float_type)M_PI)*normalizedFrequency_;
    
    //2. computation of h(n)
    //2.1. A
    float_type minOscill;
    oscillationInPass_ < oscillationInStop_ ? minOscill=oscillationInPass_ : minOscill=oscillationInStop_;
    float_type A=-20*log10(minOscill);
    
    //2.2. Beta
    float_type Beta;
    if(A<21) Beta=0;
    else {
        if(21<=A && A<=50) Beta=0.5842*pow(A-21, (float_type)0.4)+0.07886*(A-21);
        else Beta=0.1102*(A-8.7);
    }
    
    //2.3. N (length of filter)
    int N = (int)ceil ( (A-7.95)* samplingRate_/14.36/(fstop_-fpass_) );
    if(N%2==0) ++N;
    int M = (N-1)/2;
    irf_.M_=M;
    //allocation space for filter
    irf_.h_  = new float_type[N];
    irf_.N_  = N;
    irf_.numOfChannels_ = 1;
    
    //2.4. Kaiser window coefficients
    //2.4.1 I_O(Beta);	I_0 ->  is the zeroth order Modified Bessel function of the first kind
    float_type I_0_fromBetta=I_0(Beta);
    //2.4.2 Kaiser window coefficients from -M to M (2M+1==N)
    for(int n=-M; n<M+1; ++n) {
        
        //sqrt( 1-pow(((long double)(n-1)-M),2)/pow((long double)M,2))
        irf_.h_[0][n+M]=( I_0( Beta*( sqrt(1-pow(((float_type)n)/(float_type)M, 2)))   ) / I_0_fromBetta );
    }
    
    //2.5 multiply kaiser window coefficients with ideal low-pass filter
    for(int n=0; n<irf_.M_; ++n) {
        irf_.h_[0][n]*=idealLowPassFilter(n-irf_.M_);
        irf_.h_[0][2*irf_.M_-n]*=idealLowPassFilter(n-irf_.M_);
    }
    irf_.h_[0][irf_.M_]*=idealLowPassFilter(0);
    
    //ok impulseResponse[n] 0 <= n && n <= N-1
}

/*!
 equation of ideal low pass filter;
 normalized pulsation is needed;
 */
float_type IRFC_LowPass::idealLowPassFilter(int sampleNum) {
    if(sampleNum==0) return 2*normalizedFrequency_;
    return 2*normalizedFrequency_*sin(normalizedPulsation_*sampleNum)/(normalizedPulsation_*sampleNum);
}

/*!
 compute auxiliary table for I_O(float_type) function;
 Expr = 2^k * k!; result will be put in besselElem table;
 */
void IRFC_LowPass::prepareBesselAprox() {
    besselElem[0]=1;
    for(int i=1; i<numElemApprox; ++i) {
        besselElem[i]=besselElem[i-1]*2*i;
    }
}

//! zeroth order Modified Bessel function of the first kind
/*!
 aproximation made by >>numElemApprox<< first elements sumed
 computation done on long double
 */
float_type IRFC_LowPass::I_0(long double x) {
    long double besselSum=0;
    for(int i=1; i<numElemApprox; ++i) {
        besselSum+=pow(pow(x, i)/besselElem[i],2);
    }
    return besselSum+1;
}
