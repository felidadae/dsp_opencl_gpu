//
//  FMTypes.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 01.03.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//



__kernel
void in_out( __global float* in, __global float* out )
{
    int index = get_global_id(0);
    out[index] = in[index];
}
