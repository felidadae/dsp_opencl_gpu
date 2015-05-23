
#include "OpenCLProcessorBase.h"
#include <stdio.h>
//---
#include "pl_opencl_device_description.h"

bool OpenCLProcessorBase::ifPrintedDeviceInfo = false;

OpenCLProcessorBase::OpenCLProcessorBase(char* filename, bool ifPrintDeviceInfo, char* compilerMacrodefinition) {
    
    readSourceFromFile(filename);
    
    initialize(compilerMacrodefinition);
    
    if(ifPrintDeviceInfo && !ifPrintedDeviceInfo) {
        std::cout << "******************************************************" << std::endl;
        std::cout << ">>" << TITLE << "<<" << std::endl;
        std::cout << "***" << std::endl;
        printDeviceInfo();
        std::cout << "******************************************************" << std::endl << std::endl << std::endl << std::endl;
        ifPrintedDeviceInfo = true;
        sleep(3);
    }
    
}

OpenCLProcessorBase::~OpenCLProcessorBase() {
    clReleaseProgram(program_);
    clReleaseCommandQueue(cmdQueue_);
    for(int i = 0; i < numDevices_; ++i) {
        clReleaseDevice(devices_[i]);
    }
    free ( devices_ );
    free ( platforms_);
    clReleaseContext(context_);
    
}



void OpenCLProcessorBase::printDeviceInfo() {
    char name[128];
    clGetDeviceInfo(devices_[0], CL_DEVICE_NAME, 128, name, NULL);
    fprintf(stdout, DISPATCH_QUEUE ": %s\n", name);
    //
    char nameDriverVersion[128];
    clGetDeviceInfo(devices_[0], CL_DRIVER_VERSION, 128, nameDriverVersion, NULL);
    fprintf(stdout, DRIVER_OPENCL_VERSION ": %s\n", nameDriverVersion);
    //
    char versionNumber[128];
    clGetDeviceInfo(devices_[0], CL_DEVICE_VERSION, 128, versionNumber, NULL);
    fprintf(stdout, DEVICE_SUPPORTED_OPENCL_VERSION ": %s\n", versionNumber);
    //
    cl_uint computeUnitsNumber;
    clGetDeviceInfo(devices_[0], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(computeUnitsNumber), &computeUnitsNumber, NULL);
    fprintf(stdout, MAX_COMPUTE_UNITS ": %d\n", computeUnitsNumber);
    //
    cl_uint maxWorkGroupDimensions;
    clGetDeviceInfo(devices_[0], CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &maxWorkGroupDimensions, NULL);
    fprintf(stdout, MAX_WORKGROUP_DIMENSIONS ": %d\n", maxWorkGroupDimensions);
    //
    size_t maxWorkGroupSize;
    clGetDeviceInfo(devices_[0], CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &maxWorkGroupSize, NULL);
    fprintf(stdout, MAX_WORKGROUP_SIZE ": %zd\n", maxWorkGroupSize);
    //
    cl_ulong globalMemorySize;
    clGetDeviceInfo(devices_[0], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &globalMemorySize, NULL);
    fprintf(stdout, GLOBAL_MEMORY_SIZE ": %lld MB\n", globalMemorySize/1048576);
    //
    cl_ulong localMemorySize;
    clGetDeviceInfo(devices_[0], CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &localMemorySize, NULL);
    fprintf(stdout, LOCAL_MEMORY_SIZE ": %lld kB\n", localMemorySize/1024);
    //
    cl_command_queue_properties prop;
    clGetDeviceInfo(devices_[0], CL_DEVICE_QUEUE_PROPERTIES, sizeof(cl_command_queue_properties), &prop, NULL);
    fprintf(stdout, OUT_OF_ORDER_EXECUTION ": %s\n", (prop & CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE)? TRUE_TEXT: FALSE_TEXT);

}

cl_ulong OpenCLProcessorBase::getAmountOfGlobalMemory() {
    cl_ulong globalMemorySize;
    clGetDeviceInfo(devices_[0], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &globalMemorySize, NULL);
    return globalMemorySize;
}

void OpenCLProcessorBase::readSourceFromFile(char* filename) {
	struct stat statbuf;
	FILE *fh;
	
	fh = fopen(filename, "r");
	if (fh == 0)
		throw NotFoundFile_Exception();
	
	stat(filename, &statbuf);
	source_ = (char *) malloc(statbuf.st_size + 1);
	fread(source_, statbuf.st_size, 1, fh);
	source_[statbuf.st_size] = '\0';
}

void OpenCLProcessorBase::initialize(char* compilerMacrodefinition) {
    
    //-----------------------------------------------------
    // Discover and initialize the platforms
    //-----------------------------------------------------
    
    cl_uint numPlatforms = 0;
    platforms_ = NULL;
    
    // Use clGetPlatformIDs() to retrieve the number of
    // platforms
    lastCommandStatus_ = clGetPlatformIDs(0, NULL, &numPlatforms);
    
    // Allocate enough space for each platform
    platforms_ =
    (cl_platform_id*)malloc(numPlatforms*sizeof(cl_platform_id));
    
    // Fill in platforms with clGetPlatformIDs()
    lastCommandStatus_ = clGetPlatformIDs(numPlatforms, platforms_, NULL);
    
    //-----------------------------------------------------
    // Discover and initialize the devices
    //-----------------------------------------------------
    
    numDevices_ = 0;
    devices_ = NULL;
    
    // Use clGetDeviceIDs() to retrieve the number of
    // devices present
    lastCommandStatus_ = clGetDeviceIDs( platforms_[0], CL_DEVICE_TYPE_GPU, 0, NULL, &numDevices_);
    
    // Allocate enough space for each device
    devices_ =
    (cl_device_id*)malloc( numDevices_*sizeof(cl_device_id) );
    
    // Fill in devices with clGetDeviceIDs()
    lastCommandStatus_ = clGetDeviceIDs( platforms_[0], CL_DEVICE_TYPE_GPU, numDevices_, devices_, NULL);
    
    //-----------------------------------------------------
    // Create a context
    //-----------------------------------------------------
    
    // Create a context using clCreateContext() and
    // associate it with the devices
    context_ = clCreateContext( NULL, numDevices_, devices_, NULL, NULL, &lastCommandStatus_);
    
    //-----------------------------------------------------
    // Create a command queue
    //-----------------------------------------------------
    
    // Create a command queue using clCreateCommandQueue(),
    // and associate it with the device you want to execute
    // on
    cmdQueue_ = clCreateCommandQueue( context_, devices_[0], 0, &lastCommandStatus_);
    
	//-----------------------------------------------------
    // Create and compile the program
    //-----------------------------------------------------
    
    // Create a program using clCreateProgramWithSource()
    program_ = clCreateProgramWithSource(context_, 1, (const char**)&source_, NULL,&lastCommandStatus_);
    delete [] source_; //< we don't need anymore source
    
    // Build (compile) the program for the devices with
    // clBuildProgram()
    lastCommandStatus_ = clBuildProgram( program_, numDevices_, devices_, compilerMacrodefinition, NULL, NULL);
}


