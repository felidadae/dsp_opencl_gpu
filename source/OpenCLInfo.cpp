//
//  OpenCLInfo.cpp
//  DSPOpenCLGPUTester
//
//  Created by kadlubek47 on 16.01.2014.
//  Copyright (c) 2014 kadlubek47. All rights reserved.
//

#include "OpenCLInfo.h"
#include <stdio.h>
#include "pl_opencl_device_description.h"


OpenCLInfo::OpenCLInfo() {
    initialize();
}

OpenCLInfo::~OpenCLInfo() {
    for(int i = 0; i < numDevices_; ++i) {
        clReleaseDevice(devices_[i]);
    }
    free ( devices_ );
    free ( platforms_);
}



void OpenCLInfo::printDeviceInfo() {
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

cl_ulong OpenCLInfo::getAmountOfGlobalMemory() {
    cl_ulong globalMemorySize;
    clGetDeviceInfo(devices_[0], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &globalMemorySize, NULL);
    return globalMemorySize;
}

void OpenCLInfo::initialize() {
    
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
}


