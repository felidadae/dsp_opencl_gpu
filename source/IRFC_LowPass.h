//
//  LowPass_IRFC.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 01.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef LowPassFilter_Standalone_IRFC_LowPass_h
#define LowPassFilter_Standalone_IRFC_LowPass_h

#include "IRFCBase.h"   //< Base class



class IRFC_LowPass: public IRFCBase {
public:
    IRFC_LowPass        ();
    void setParameters  (const FMParameters& params);
    
private:
    //Parameters
    float_type oscillationInPass_;
	float_type oscillationInStop_;
    unsigned int fpass_, fstop_;
	unsigned int  samplingRate_;
    //.
    
    void computeImpulseResponse();
    
    //------------------------------------------------------------------------------
    float_type normalizedPulsation_, normalizedFrequency_;
    static const int numElemApprox=32;              //!< number of elements which will approximate sum
	long double besselElem[numElemApprox];	//!< auxiliary table for I_O(float_type) function
    
	/*!
     equation of ideal low pass filter;
     normalized pulsation is needed;
     */
	float_type idealLowPassFilter(int sampleNum);
	
	/*!
     compute auxiliary table for I_O(float_type) function;
     Expr = 2^k * k!; result will be put in besselElem table;
     */
	void prepareBesselAprox();
    
	/*!
     zeroth order Modified Bessel function of the first kind
     
     aproximation made by >>numElemApprox<< first elements sumed
     computation done on long double
     */
	float_type I_0(long double x);
    //------------------------------------------------------------------------------
};



#endif
