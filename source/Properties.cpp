//
//  Properties.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 21.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "Properties.h"
#include "language_pl.h"

#define str(x) std::string(x)



const std::string Properties::experiment_title              = str(PROPERTIE_NAME__EXPERIMENT_TITLE);
//---
const std::string Properties::lengthInSeconds               = str(PROPERTIE_NAME__LENGTH_IN_SECONDS);
const std::string Properties::repetition                    = str(PROPERTIE_NAME__REPETITION);
const std::string Properties::block_size                    = str(PROPERTIE_NAME__BLOCKS_SIZE);
const std::string Properties::sample_rate                   = str(PROPERTIE_NAME__SAMPLE_RATE);
const std::string Properties::num_of_channels               = str(PROPERTIE_NAME__NUMBER_OF_CHANNELS);
const std::string Properties::ganged_or_multi_buffer        = str(PROPERTIE_NAME__GANGED_OR_MULTI_BUFFER);
const std::string Properties::work_name                     = str(PROPERTIE_NAME__WORK_NAME);
const std::string Properties::latency                       = str(PROPERTIE_NAME__LATENCY);
//---
const std::string Properties::allocatedGPUMemory            = str(PROPERTIE_NAME__ALLOCATED_GPU_MEMORY);
const std::string Properties::gpuThreadsNumber              = str(PROPERTIE_NAME__GPU_THREADS_NUMBER);

//---
const std::string Properties::averageTimePerSample          = str(PROPERTIE_NAME__AVERAGE_TIME_PER_SAMPLE);
const std::string Properties::average                       = str(PROPERTIE_NAME__AVERAGE);
const std::string Properties::minimum                       = str(PROPERTIE_NAME__MINIMUM);
const std::string Properties::median                        = str(PROPERTIE_NAME__MEDIAN);
const std::string Properties::_99                           = str(PROPERTIE_NAME__99PROCENT);
const std::string Properties::maximum                       = str(PROPERTIE_NAME__MAXIMUM);
const std::string Properties::onTime                        = str(PROPERTIE_NAME__ON_TIME);
const std::string Properties::onTimePercentage              = str(PROPERTIE_NAME__PERCENTAGE_ON_TIME);
//---
const std::string Properties::impulse_response_N            = str(PROPERTIE_NAME__IMPULSE_RESPONSE_N);
const std::string Properties::impulse_response_C            = str(PROPERTIE_NAME__IMPULSE_RESPONSE_C);
const std::string Properties::convolutionerType             = str(PROPERTIE_NAME__CONVOLUTIONER);
const std::string Properties::num_mainKernel_LoopIteration  = str(PROPERTIE_NAME__MAIN_KERNEL_LOOP_ITERATION);
