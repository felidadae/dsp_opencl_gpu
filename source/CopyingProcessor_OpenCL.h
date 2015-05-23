//
//  CopyingProcessor_OpenCL.h
//  GPUProcessingTesting
//
//  Created by kadlubek47 on 17.06.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __GPUProcessingTesting__CopyingProcessor_OpenCL__
#define __GPUProcessingTesting__CopyingProcessor_OpenCL__

#include "OpenCLProcessorBase.h"
#include "IProcess.h"
#include "OpenCLBuffer.h"
#include "Timer.h"
//---
#include <vector>



//Multi channel testing processor - one buffer for each channel (many channels)
class CopyingProcessor_OpenCL: public OpenCLProcessorBase, public IProcess<float> {
public:
    
    CopyingProcessor_OpenCL();
    ~CopyingProcessor_OpenCL();
    
    void process( AudioInOutBuffers<float>& audioBlocks );
    
private:
    //Buffers
    std::vector<OpenCLBuffer<float>*> buffersIn_, buffersOut_;
    //.
    
    //Kernel
    static const char kernelPath_[];
    cl_kernel inOutKernel_;
    //.
};

#endif /* defined(__GPUProcessingTesting__CopyingProcessor_OpenCL__) */







