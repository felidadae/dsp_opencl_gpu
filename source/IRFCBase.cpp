//
//  IRFCBase.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 23.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "IRFCBase.h"
#include <cstring>                      //< for memcpy
#include "FMTypes.h"          //< Data type for Impulse Response values



const IRF* IRFCBase::getPointerToIRF() {
    return &irf_;
}

void IRFCBase::deleteIRF() {
    if(irf_.h_.data_)
        delete [] irf_.h_.data_;
}
