//
//  OpenCLInfo.h
//  DSPOpenCLGPUTester
//
//  Created by kadlubek47 on 16.01.2014.
//  Copyright (c) 2014 kadlubek47. All rights reserved.
//

#ifndef __DSPOpenCLGPUTester__OpenCLInfo__
#define __DSPOpenCLGPUTester__OpenCLInfo__

#include <OpenCL/opencl.h>
#include <sys/stat.h>

class OpenCLInfo {
public:
    OpenCLInfo();
    ~OpenCLInfo();
    
    void printDeviceInfo();
    cl_ulong getAmountOfGlobalMemory();
    
private:
    void initialize();
    
    cl_int lastCommandStatus_;

    cl_uint numDevices_;
    cl_device_id *devices_;
    cl_platform_id *platforms_;
};


#endif /* defined(__DSPOpenCLGPUTester__OpenCLInfo__) */
