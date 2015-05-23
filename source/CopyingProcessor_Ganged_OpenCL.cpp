//
//  CopyingProcessor_Ganged_OpenCL.cpp
//  GPUProcessingTesting
//
//  Created by kadlubek47 on 17.06.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "CopyingProcessor_Ganged_OpenCL.h"



#pragma mark Create, Destroy

const char CopyingProcessor_Ganged_OpenCL::kernelPath_[] = "simply_kernels.cl";

CopyingProcessor_Ganged_OpenCL::CopyingProcessor_Ganged_OpenCL(): OpenCLProcessorWithBuffersInOutBase<float>((char*)kernelPath_) {
    inOutKernel_ = clCreateKernel(program_, "in_out", &lastCommandStatus_);
}

CopyingProcessor_Ganged_OpenCL::~CopyingProcessor_Ganged_OpenCL() {
    clReleaseKernel(inOutKernel_);
}



#pragma mark manyChannel process

void CopyingProcessor_Ganged_OpenCL::process( AudioInOutBuffers<float>& audio )
{
    unsigned int allLength = audio.in_.get_allLength();
    
    //Check if recreate
    if ( allLength != bufferIn_.getLength() ) {
        /***/bufferIn_.recreate(CL_MEM_READ_ONLY,  allLength );
        /**/bufferOut_.recreate(CL_MEM_WRITE_ONLY, allLength );
        
        /***/bufferIn_.setAsKernelArgument(0, inOutKernel_);
        /**/bufferOut_.setAsKernelArgument(1, inOutKernel_);
    }
    //.

    //Transfer to GPU
    bufferIn_.set(audio.in_.data_);
    //.
    
    //Execute kernel
    size_t globalWorkSize[1];
    globalWorkSize[0] = allLength;
    lastCommandStatus_ = clEnqueueNDRangeKernel( cmdQueue_, inOutKernel_, 1, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    //.
    
    //Transfer from GPU to CPU
    bufferOut_.get(audio.out_.data_);
    //.

    //Flushing and finishing
    clFlush(cmdQueue_);
    clFinish(cmdQueue_);
    //.
}


