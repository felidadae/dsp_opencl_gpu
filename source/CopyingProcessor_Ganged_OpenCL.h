//
//  CopyingProcessor_Ganged_OpenCL.h
//  GPUProcessingTesting
//
//  Created by kadlubek47 on 17.06.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __GPUProcessingTesting__CopyingProcessor_Ganged_OpenCL__
#define __GPUProcessingTesting__CopyingProcessor_Ganged_OpenCL__

#include "OpenCLProcessorWithBuffersInOutBase.h"
#include "IProcess.h"
#include "Timer.h"



//Multi channel testing processor - one buffer for all channels
class CopyingProcessor_Ganged_OpenCL: public OpenCLProcessorWithBuffersInOutBase<float>, public IProcess<float>  {
public:
    
    CopyingProcessor_Ganged_OpenCL();
    ~CopyingProcessor_Ganged_OpenCL();
    
    void process( AudioInOutBuffers<float>& audioBlocks );
    
private:
    //Kernel
    static const char kernelPath_[];
    cl_kernel inOutKernel_;
    //.
};

#endif /* defined(__GPUProcessingTesting__CopyingProcessor_Ganged_OpenCL__) */







