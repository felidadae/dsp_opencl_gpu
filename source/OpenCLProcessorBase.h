
#ifndef __LowPassFilter_Standalone__OpenCLProcessorBase__
#define __LowPassFilter_Standalone__OpenCLProcessorBase__

#include <iostream>

#include "GeneralExceptions.h"
#include <OpenCL/opencl.h>
#include <sys/stat.h>



/*
    Creates base for opencl >>GPU<< processing;
    Create context, command queue, program(program from file given in constructor);
    Kernels from program must be created in function createKernels();
 */
class OpenCLProcessorBase {
public:
    OpenCLProcessorBase(char* filename /* opencl code file */, bool ifPrintDeviceInfo = false, char* compilerMacrodefinition = nullptr);
    ~OpenCLProcessorBase();
    
    void printDeviceInfo();
    cl_ulong getAmountOfGlobalMemory();
    
protected:
    cl_int lastCommandStatus_;
    
    cl_context context_;
	cl_command_queue cmdQueue_;
    cl_program program_;
    
private:
    void initialize(char* compilerMacrodefinition);
    void readSourceFromFile(char* filename);

    char* source_;

    cl_uint numDevices_;
    cl_device_id *devices_;
    cl_platform_id *platforms_;
    
    static bool ifPrintedDeviceInfo;
};



#endif /* defined(__LowPassFilter_Standalone__OpenCLProcessorBase__) */
