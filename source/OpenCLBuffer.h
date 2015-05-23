//
//  OpenCLProcessorBaseWithBuffersInOut.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 26.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__OpenCLProcessorBaseWithBuffersInOut__
#define __FilterModule_Standalone__OpenCLProcessorBaseWithBuffersInOut__

#include <OpenCL/opencl.h>
#include "OpenCLProcessorBase.h"



template<class ElementType>
class OpenCLBuffer {
public:
    
    OpenCLBuffer (cl_context& context, cl_command_queue& cmdQueue, cl_int* lastCommandStatus): context_(context), cmdQueue_(cmdQueue), lastCommandStatus_(lastCommandStatus) {}
    OpenCLBuffer (const OpenCLBuffer& b): context_(b.context_), cmdQueue_(b.cmdQueue_), lastCommandStatus_(b.lastCommandStatus_)  {
        created_ = b.created_;
        length_  = b.length_;
    }
    OpenCLBuffer& operator=(const OpenCLBuffer& b) {
        created_ = b.created_;
        length_  = b.length_;
        context_ = b.context_;
        cmdQueue_= b.cmdQueue_;
        lastCommandStatus_ = b.lastCommandStatus_;
        return *this;
    }
    ~OpenCLBuffer () {if(created_) clReleaseMemObject(buffer_);}
    
    bool ifCreated          ()                                                  {return created_;}
    void recreate           (cl_mem_flags creationFlag, unsigned int newLength) {
        length_ = newLength;
        if(created_)
            clReleaseMemObject(buffer_);
        
        buffer_  = clCreateBuffer(context_, creationFlag, length_ * sizeof(ElementType), NULL, lastCommandStatus_);
        created_ = true;
    }
    
    void fillWithZero       ()                                                  {
        ElementType v = 0;
        clEnqueueFillBuffer(cmdQueue_, buffer_, &v, sizeof(ElementType), 0, length_ * sizeof(ElementType), 0, NULL, NULL);
    }
    void set                (ElementType* data   )                              {
        *lastCommandStatus_ = clEnqueueWriteBuffer   (cmdQueue_, buffer_, CL_FALSE, 0, length_ * sizeof(ElementType), data,      0, NULL, NULL);
    }
    void get                (ElementType* destiny)                              {
        *lastCommandStatus_ = clEnqueueReadBuffer    (cmdQueue_, buffer_, CL_FALSE, 0, length_ * sizeof(ElementType), destiny,   0, NULL, NULL);
    }
    
    cl_mem& getClMemObject  ()                                                  {return buffer_;}
    operator cl_mem& ()                                                         {
        return buffer_; }
    
    void setAsKernelArgument(unsigned int argumentIndex, cl_kernel& kernel)     {
        *lastCommandStatus_  = clSetKernelArg (kernel, argumentIndex, sizeof(cl_mem), &buffer_);
    }
    
    unsigned int getLength  () {return length_;}
    
private:
    //Main
    bool created_           = false;
    unsigned int length_    = 0;
    cl_mem buffer_;
    //.
    
    //OpenCL environment
    cl_context&         context_;
    cl_command_queue&   cmdQueue_;
    cl_int*            lastCommandStatus_;
    //.
};



#endif /* defined(__FilterModule_Standalone__OpenCLProcessorBaseWithBuffersInOut__) */
