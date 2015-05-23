//
//  OpenCLProcessorWithBuffersInOutBase.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 26.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__OpenCLProcessorWithBuffersInOutBase__
#define __FilterModule_Standalone__OpenCLProcessorWithBuffersInOutBase__

#include "OpenCLProcessorBase.h"
#include "OpenCLBuffer.h"

template<class BufferElementType>
class OpenCLProcessorWithBuffersInOutBase: public OpenCLProcessorBase {
public:
    OpenCLProcessorWithBuffersInOutBase(char* programFileName): OpenCLProcessorBase(programFileName), bufferIn_ (context_, cmdQueue_, &lastCommandStatus_), bufferOut_(context_, cmdQueue_, &lastCommandStatus_) { }
    
protected:
    //Buffers in, out
    OpenCLBuffer<BufferElementType> bufferIn_, bufferOut_;
    //.
};

#endif /* defined(__FilterModule_Standalone__OpenCLProcessorWithBuffersInOutBase__) */
