//
//  Laboratory.cpp
//  GPUProcessingTesting
//
//  Created by kadlubek47 on 12.07.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//


#include "Laboratory.h"
//---
#include "FMTypes.h"
#include "FilterModule.h"
//---
#include "CopyingProcessor_Ganged_OpenCL.h"
#include "CopyingProcessor_OpenCL.h"
//---
#include "AudioInOutBuffers.h"
#include "AudioStream.h"
#include "GeneralExceptions.h"
//---
#include <sstream>
//--
#include "language_pl.h"
#include "StatisticsComputator.h"
//
#include <cmath>
#include "OpenCLInfo.h"
#include "FMTypes.h"

#include <cstdlib>
#include <sys/stat.h>







#pragma mark Contructor
std::string Laboratory::InputAudioParameters::inFileName    = std::string();
std::string Laboratory::InputFMParameters::irFilePath       = std::string();
std::string findValueOfField( std::string fieldName, std::string text ) {
    std::size_t fieldPos = text.find(fieldName);
    std::size_t beginValueOfField = text.find( std::string("\""), fieldPos );
    std::size_t endValueOfField   = text.find( std::string("\""), beginValueOfField + 1 );
    
    std::string valueOfField = text.substr(beginValueOfField+1, endValueOfField-beginValueOfField-1);
    return valueOfField;
}
bool makeBoolFromString( std::string s ) {
    if (s == std::string("YES"))
        return true;
    else
        return false;
}
unsigned int makeUnsignedFromString( std::string s ) {
    return atoi(s.c_str());
}
Laboratory::Laboratory() {
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //READ
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //read in.txt file
    using namespace std;
    string inFileString;
	ifstream inFile;
	inFile.open("in.txt");
	if(!inFile.is_open()) {
		cout<<"File with input commands must exist with name >>in.txt<< \n"<<std::endl;
		return;
	}
	inFile.seekg(0, std::ios::end);
	inFileString.resize((int)inFile.tellg());
	inFile.seekg(0, std::ios::beg);
	inFile.read(&inFileString[0], inFileString.size());
    //.
    
    std::string resultsPath;
    std::string inAudioFilePath;
    std::string impulseResponsePath;
    //---
    bool ifCarryExperiments;
    //---
    std::string hardware;
    unsigned int blockSize;
    
    std::string name_resultsPath            ("RESULTS_PATH");
    std::string name_inAudioFilePath        ("AUDIO_IN_FILE_PATH");
    std::string name_impulseResponsePath    ("IMPULSE_RESPONSE_PATH");
    //---
    std::string name_ifCarryExperiments     ("IF_CARRY_EXPERIMENTS");
    //---
    std::string name_blockSize              ("BLOCK_SIZE");
    std::string name_hardware               ("HARDWARE");
    
    resultsPath         = findValueOfField(name_resultsPath                             , inFileString   );
    inAudioFilePath     = findValueOfField(name_inAudioFilePath                         , inFileString   );
    impulseResponsePath = findValueOfField(name_impulseResponsePath                     , inFileString   );
    //---
    ifCarryExperiments  = makeBoolFromString(findValueOfField(name_ifCarryExperiments   , inFileString  ));
    //---
    blockSize           = makeUnsignedFromString((findValueOfField(name_blockSize       , inFileString )));
    hardware            = findValueOfField(name_hardware                                , inFileString   );
    
    resultsPath_ = resultsPath;
    Laboratory::InputAudioParameters::inFileName    = inAudioFilePath;
    Laboratory::InputFMParameters::irFilePath       = impulseResponsePath;
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    //MAKE EXPERIMENTS
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    startComputations(ifCarryExperiments, blockSize, hardware);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
}
void Laboratory::startComputations(bool ifCarryExperiments, unsigned int blockSize, std::string hardware) {
    if(ifCarryExperiments) {
        audioTest();
        std::cout<< "audioTest done \n";
        multiChannelExperiment();
        std::cout<< "multiChannelExperiment done \n";
        minimumBufferSizeExperiment();
        std::cout<< "minimumBufferSize done \n";
        //---
        FMAudioTest();
        std::cout<< "FMAudioTest done \n";
        FMGPUGeneral();
        std::cout<< "FMGPUGeneral done \n";
        FMComparison();
        std::cout<< "FMComparison done \n";
        FMMax(FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
        FMMax(FMParamsInfo::NORMAL_CPU__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
        std::cout<< "FMMax done \n";
    }
    else {
        mkdir((resultsPath_ + std::string("singleProcessing") ).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        InputAudioParameters str;
        str.blockSize = blockSize;
        str.data_source = AudioStream::AUDIO_FILE_AS_SOURCE;
        Laboratory::InputFMParameters fm_str(str);
        if(hardware == std::string("GPU"))
            fm_str.params_.add(FMParamsInfo::convolutionerID, FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
        else
            fm_str.params_.add(FMParamsInfo::convolutionerID, FMParamsInfo::NORMAL_CPU__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
        
        FMMain_testFunction(str, fm_str, std::string("singleProcessing"));
    }
}









#pragma mark Help funcions

void Laboratory::addStandardAndStatisticsProperties(TrialsSet& trialsSet, AudioStream& audioStream, InputAudioParameters& str  ) {
    trialsSet.properties_.add( Properties::lengthInSeconds,             (unsigned int)  ceil(((double)str.blockSize*trialsSet.get_trialsNumber())/((double)audioStream.get_sampleRate()))   );
    trialsSet.properties_.add( Properties::repetition,                  (unsigned int)  trialsSet.get_trialsNumber()                                                );
    trialsSet.properties_.add( Properties::block_size,                  (unsigned int)  str.blockSize                                                               );
    trialsSet.properties_.add( Properties::sample_rate,                 (unsigned int)  audioStream.get_sampleRate()                                                );
    trialsSet.properties_.add( Properties::latency,                     (double)        (double)str.blockSize/(double)audioStream.get_sampleRate()*(double)1000     );
    trialsSet.properties_.add( Properties::num_of_channels,             (unsigned int)  audioStream.get_numOfChannels()                                             );
    //---
    StatisticsComputator::compute(trialsSet);
    //---
}

void Laboratory::checkAndSave(TrialsSet& trialsSet, AudioStream& audioStream, InputAudioParameters& str, CheckMethodType checkMethod, std::string folderForAudioResults ) {
    //Check - write result to file
    if(str.data_source == AudioStream::AUDIO_FILE_AS_SOURCE) {
        audioStream.close       (resultsPath_  + folderForAudioResults + std::string("/")  + str.outFileName, true);
        audioStream.cutAndClose (resultsPath_  + folderForAudioResults + std::string("/")  + std::string("cutted__") +str.outFileName, trialsSet, false );
    }
    if(str.data_source == AudioStream::RANDOM_DATA_AS_SOURCE)
        if(checkMethod == CheckMethodType::ifOutSilence)
            if(audioStream.checkIfOutSilence())
                throw UnkownInternalProcessingError_Exception();
    
    if(checkMethod == CheckMethodType::ifTheInTheSameAsOut)
        if(!audioStream.checkIfOutTheSameAsIn())
            throw UnkownInternalProcessingError_Exception();
    //.
}










#pragma mark Experiments

void Laboratory::FMAudioTest()  {
    using namespace std;
    
    Experiment experiment( "FMAudioTest", resultsPath_ );
    
    //Making experiment
    {
        //Set parameters
        InputAudioParameters str;
        Laboratory::InputFMParameters fm_str(str);
        //...
        //.
        
        //Execute test function
        str.data_source = AudioStream::AUDIO_FILE_AS_SOURCE;
        for(str.blockSize = 128; str.blockSize <= 1024; str.blockSize*=2) {
            
            //cpu
            std::stringstream s; s   << "blockSize-" << str.blockSize << "_" << "CPU" << ".wav";
            str.outFileName = s.str();
            fm_str.params_.add(FMParamsInfo::convolutionerID, FMParamsInfo::NORMAL_CPU__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
            experiment.add_trialsSet( FMMain_testFunction(str, fm_str, experiment.title_) );
            
            //opencl gpu
            std::stringstream ss; ss << "blockSize-" << str.blockSize << "_" << "OpenCLGPU" << ".wav";
            str.outFileName = ss.str();
            fm_str.params_.add(FMParamsInfo::convolutionerID, FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
            experiment.add_trialsSet( FMMain_testFunction(str, fm_str, experiment.title_) );
            
        }
        //.
    }
    //.
    
    //Setting properties
    //ADD
    experiment.properties_.add                         (Properties::experiment_title, std::string(PROPERTIE_VALUE__EXPERIMENT_TITLE__FM_AUDIO_TEST));
    //---
    //MAIN
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::num_of_channels    );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::impulse_response_N );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::impulse_response_C );
    //---
    //Each TrialSet
    experiment.set_trialsSet_propierty_to_print        (Properties::block_size         );
    experiment.set_trialsSet_propierty_to_print        (Properties::convolutionerType  );
    experiment.set_trialsSet_propierty_to_print        (Properties::latency            );
    //...
    experiment.set_trialsSet_propierty_to_print        (Properties::median             );
    experiment.set_trialsSet_propierty_to_print        (Properties::_99                );
    experiment.set_trialsSet_propierty_to_print        (Properties::repetition         );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTime             );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTimePercentage   );
    //.
    
    //Writing results
    experiment.writeResultsTo_stdcout          (false);
    experiment.writeResultsTo_file_makeRformat      ();
    experiment.write_toFile_table_latexTabularFormat();
    //.
}

void Laboratory::FMComparison() {
    using namespace std;
    
    Experiment experiment( "FMComparison", resultsPath_ );
    
    //Making experiment
    {
        //Set parameters
        InputAudioParameters str;
        Laboratory::InputFMParameters fm_str(str);
        str.data_source = AudioStream::RANDOM_DATA_AS_SOURCE;
        str.lengthInSeconds = 20;
        //.
        
        unsigned int channArray[] = {1,2,8,16};
        unsigned int irNArray[]    = {100, 5000, 44100};
        unsigned int blockArray[] = {128, 512, 2048};
        
        for (unsigned i = 0; i < 4; ++i)
            for (unsigned j = 0; j < 3; ++j)
                for(unsigned int k = 0; k < 3; ++k)
                {
                    //set
                    str.virtual_numOfChannels = channArray[i];
                    str.virtual_irN = irNArray[j];
                    str.blockSize = blockArray[k];
                    //.
                    
                    //Set specific params
                    str.virtual_numOfBlocks = str.sampleRate * str.lengthInSeconds / str.blockSize;
                    fm_str.update(str);
                    //.
                    
                    //cpu
                    fm_str.params_.add(FMParamsInfo::convolutionerID, FMParamsInfo::NORMAL_CPU__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
                    experiment.add_trialsSet( FMMain_testFunction(str, fm_str, experiment.title_) );
                    //.
                    
                    //opencl gpu
                    fm_str.params_.add(FMParamsInfo::convolutionerID, FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
                    experiment.add_trialsSet( FMMain_testFunction(str, fm_str, experiment.title_) );
                    //.
                }
    }
    //.
    
    //Setting properties
    //ADD
    experiment.properties_.add                         (Properties::experiment_title, std::string(PROPERTIE_VALUE__EXPERIMENT_TITLE__FM_COMPARISON));
    //---
    //MAIN
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::lengthInSeconds    );
    //---
    //Each TrialSet
    experiment.set_trialsSet_propierty_to_print        (Properties::impulse_response_N );
    experiment.set_trialsSet_propierty_to_print        (Properties::num_of_channels    );
    experiment.set_trialsSet_propierty_to_print        (Properties::block_size         );
    experiment.set_trialsSet_propierty_to_print        (Properties::convolutionerType  );
    //...
    experiment.set_trialsSet_propierty_to_print        (Properties::averageTimePerSample    );
    experiment.set_trialsSet_propierty_to_print        (Properties::average                 );
    experiment.set_trialsSet_propierty_to_print        (Properties::median                  );
    experiment.set_trialsSet_propierty_to_print        (Properties::_99                     );
    experiment.set_trialsSet_propierty_to_print        (Properties::maximum                 );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTime                  );
    experiment.set_trialsSet_propierty_to_print        (Properties::repetition              );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTimePercentage        );
    //.
    
    //Writing results
    experiment.writeResultsTo_stdcout          (false);
    experiment.writeResultsTo_file_makeRformat ();
    experiment.write_toFile_table_latexTabularFormat_comparison();
    //.
}

void Laboratory::FMGPUGeneral() {
    using namespace std;
    
    Experiment experiment( "FMGPUGeneral", resultsPath_ );
    
    //Making experiment
    {
        //Set parameters
        InputAudioParameters str;
        Laboratory::InputFMParameters fm_str(str);
        str.data_source = AudioStream::RANDOM_DATA_AS_SOURCE;
        str.lengthInSeconds = 20;
        //.
        
        unsigned int    irArray[] = {100, 5000, 44100};
        unsigned int channArray[] = {2, 8, 32};
        unsigned int blockArray[] = {128, 256, 512, 1024, 2048};
        //---
        for(unsigned i = 0; i < 3; ++i)
            for (unsigned j = 0; j < 3; ++j)            //channNumber
                for(unsigned int k = 0; k < 5; ++k)     //blockSize
                {
                    //set
                    str.virtual_irN             =    irArray[i];
                    str.virtual_numOfChannels   = channArray[j];
                    str.blockSize               = blockArray[k];
                    //.
                    
                    //Set specific params
                    str.virtual_numOfBlocks = str.sampleRate * str.lengthInSeconds / str.blockSize;
                    fm_str.update(str);
                    //.
                    
                    //opencl gpu
                    fm_str.params_.add(FMParamsInfo::convolutionerID, FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED);
                    experiment.add_trialsSet( FMMain_testFunction(str, fm_str, experiment.title_) );
                    //.
                }
    }
    //.
    
    //Setting properties
    //ADD
    experiment.properties_.add                         (Properties::experiment_title, std::string(PROPERTIE_VALUE__EXPERIMENT_TITLE__FM_GPU_GENERAL));
    //---
    //MAIN
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::convolutionerType       );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::lengthInSeconds         );
    //---
    //Each TrialSet
    experiment.set_trialsSet_propierty_to_print        (Properties::impulse_response_N      );
    experiment.set_trialsSet_propierty_to_print        (Properties::num_of_channels         );
    experiment.set_trialsSet_propierty_to_print        (Properties::block_size              );
    //...
    experiment.set_trialsSet_propierty_to_print        (Properties::averageTimePerSample    );
    experiment.set_trialsSet_propierty_to_print        (Properties::average                 );
    experiment.set_trialsSet_propierty_to_print        (Properties::median                  );
    experiment.set_trialsSet_propierty_to_print        (Properties::_99                     );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTime                  );
    experiment.set_trialsSet_propierty_to_print        (Properties::repetition              );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTimePercentage        );
    //.
    
    //Writing results
    experiment.writeResultsTo_stdcout          (false);
    experiment.writeResultsTo_file_makeRformat ();
    experiment.write_toFile_table_latexTabularFormat(true);
    //.
}

#define AMOUNT_OF_USED_GPU_MEMORY(C, B, N) 32*C*(N+B)   //32bit for one sample

void Laboratory::FMMax(FMParamsInfo::convolutionerIDType convolutionerID) {
    //checkIfGPU
    bool ifGPU;
    std::string hardware;
    if(convolutionerID == FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED) { ifGPU = true; hardware = "GPU"; }
    else { ifGPU = false; hardware = "CPU"; }
    //.
    
    //get amount of global memory if GPU
    cl_ulong amountOfGlobalMemory; //in bytes
    if(ifGPU)
    {
        OpenCLInfo info;
        amountOfGlobalMemory = info.getAmountOfGlobalMemory();
    }
    //.
    
    using namespace std;
    string titleOfExperiment("FMMax");
    Experiment experiment(titleOfExperiment.c_str() , resultsPath_ );
    
    //Making experiment
    {
        //Set parameters
        InputAudioParameters str;
        Laboratory::InputFMParameters fm_str(str);
        fm_str.params_.add(FMParamsInfo::convolutionerID, convolutionerID);
        str.data_source = AudioStream::RANDOM_DATA_AS_SOURCE;
        str.lengthInSeconds = 20;
        str.virtual_numOfBlocks = str.sampleRate * str.lengthInSeconds / str.blockSize;
        fm_str.update(str);
        //.
        
        unsigned int blockSize_array[]  = {512, 1024, 2048};
        unsigned int N_IR_array[]       = {22500, 44100*8, 44100*16};
        //unsigned int C_AA[] = {81,9,4};
        //---
        for(unsigned b = 0; b < 3; ++b)
            for(unsigned i = 0; i < 3 ; ++i ) {
                //set
                str.virtual_irN             = N_IR_array[i];
                str.blockSize = blockSize_array[b];
                str.virtual_numOfBlocks = str.sampleRate * str.lengthInSeconds / str.blockSize;
                fm_str.update(str);
                //.
                
                bool ifLackOfGlobalMemory = false, ifFoundBoundary = false;
                unsigned int CMin = 1;
                unsigned int C = CMin ;
                TrialsSet previousTrialSet;
                while(!ifLackOfGlobalMemory && !ifFoundBoundary) {
                    if( (convolutionerID == FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED) && AMOUNT_OF_USED_GPU_MEMORY(C, blockSize_array[b], N_IR_array[i]) > 0.8*amountOfGlobalMemory) {
                        ifLackOfGlobalMemory = false;
                    }
                    else {
                        //set
                        str.virtual_numOfChannels = C;
                        fm_str.update(str);
                        //.
                        
                        //opencl gpu
                        fm_str.params_.add(FMParamsInfo::convolutionerID, convolutionerID);
                        TrialsSet currentTrialSet = FMMain_testFunction(str, fm_str, experiment.title_);
                        StatisticsComputator::compute(currentTrialSet);
                        if( (boost::any_cast<double>((currentTrialSet.properties_).get(Properties::onTimePercentage).first)) < 99) {
                            currentTrialSet = FMMain_testFunction(str, fm_str, experiment.title_);
                            StatisticsComputator::compute(currentTrialSet);
                            if( (boost::any_cast<double>((currentTrialSet.properties_).get(Properties::onTimePercentage).first)) < 99) {
                                if(C != CMin)
                                    experiment.add_trialsSet(previousTrialSet);
                                ifFoundBoundary = true;
                            }
                        }
                        previousTrialSet = currentTrialSet;
                        if(i==0)
                            C+=3;
                        else
                            C += 1;
                        //.
                    }
                }
            }
        
    }
    //.
    
    //Setting properties
    //ADD
    experiment.properties_.add                         (Properties::experiment_title, std::string(PROPERTIE_VALUE__EXPERIMENT_TITLE__FM_MAX));
    //---
    //MAIN
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::convolutionerType       );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::lengthInSeconds         );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::block_size              );
    //---
    //Each TrialSet
    experiment.set_trialsSet_propierty_to_print        (Properties::impulse_response_N      );
    experiment.set_trialsSet_propierty_to_print        (Properties::num_of_channels         );
    
    //...
    experiment.set_trialsSet_propierty_to_print        (Properties::averageTimePerSample    );
    experiment.set_trialsSet_propierty_to_print        (Properties::average                 );
    experiment.set_trialsSet_propierty_to_print        (Properties::median                  );
    experiment.set_trialsSet_propierty_to_print        (Properties::_99                     );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTime                  );
    experiment.set_trialsSet_propierty_to_print        (Properties::repetition              );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTimePercentage        );
    //.
    
    //Writing results
    experiment.writeResultsTo_stdcout          (false);
    experiment.writeResultsTo_file_makeRformat (std::string("_")+hardware);
    experiment.write_toFile_table_latexTabularFormat_Max(ifGPU);
}

//--------------------------------------------------

void Laboratory::multiChannelExperiment()      {
    
    Experiment experiment( "multiChannel", resultsPath_ );
    
    //Making experiment
    {
        //Set parameters
        InputAudioParameters str;
        str.blockSize = 512;
        str.data_source = AudioStream::RANDOM_DATA_AS_SOURCE;
        str.virtual_numOfBlocks = 1500;
        //.
        
        //Execute test function
        for(unsigned int channNum = 2; channNum <= 128; channNum*=2) {
            str.virtual_numOfChannels = channNum;
            experiment.add_trialsSet    (         bufferForEachChannel_testFunction (str, experiment.title_)   );
            experiment.add_trialsSet    (      oneBufferForAllChannels_testFunction (str, experiment.title_)   );

        }
        //.
    }
    //.
    
    //Setting properties
    //Add
    experiment.properties_.add                         (Properties::experiment_title, std::string(PROPERTIE_VALUE__EXPERIMENT_TITLE__GANGED_OR_MULTI_CHANNEL));
    //.
    //Main
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::work_name               );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::block_size              );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::repetition              );
    //.
    //Print
    experiment.set_trialsSet_propierty_to_print        (Properties::num_of_channels         );
    experiment.set_trialsSet_propierty_to_print        (Properties::ganged_or_multi_buffer  );
    //...
    experiment.set_trialsSet_propierty_to_print        (Properties::median             );
    experiment.set_trialsSet_propierty_to_print        (Properties::_99                );
    experiment.set_trialsSet_propierty_to_print        (Properties::repetition         );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTime             );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTimePercentage   );
    //.
    //.

    //Writing results
    experiment.writeResultsTo_stdcout          (false);
    experiment.writeResultsTo_file_makeRformat ();
    experiment.write_toFile_table_latexTabularFormat_comparison();
    //.
}

void Laboratory::minimumBufferSizeExperiment() {
    
    Experiment experiment( "minimumBufferSize", resultsPath_ );
    
    //Making experiment
    {
        //Set general parameters
        InputAudioParameters str;
        str.data_source = AudioStream::RANDOM_DATA_AS_SOURCE;
        str.virtual_numOfChannels = 2;
        str.lengthInSeconds = 20;
        //.
        
        for (str.virtual_numOfChannels = 2; str.virtual_numOfChannels <= 128; str.virtual_numOfChannels *= 2) {
            for(str.blockSize = /*MINIMUM BLOCK SIZE*/16; str.blockSize <= /*MAX BLOCK SIZE*/2048; str.blockSize *= 2)
            {
                //Set specific params
                str.virtual_numOfBlocks = str.sampleRate * str.lengthInSeconds / str.blockSize;
                //.
                
                //Execute test function
                //if(str.blockSize==16 && str.virtual_numOfChannels==2)
                experiment.add_trialsSet(      oneBufferForAllChannels_testFunction (str, experiment.title_)       );
                //std::cout<<"Ukończone " << str.virtual_numOfChannels << " " << str.blockSize << std::endl;
                //.
                
            }
        }
        
    }
    //.
    
    //Setting properties
    //Add
    experiment.properties_.add                         (Properties::experiment_title, std::string(PROPERTIE_VALUE__EXPERIMENT_TITLE__MINIMUM_BUFFER_SIZE));
    //.
    //Main
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::work_name               );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::ganged_or_multi_buffer  );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::lengthInSeconds         );
    //.
    //Print
    experiment.set_trialsSet_propierty_to_print        (Properties::num_of_channels         );
    experiment.set_trialsSet_propierty_to_print        (Properties::block_size              );
    //-
    experiment.set_trialsSet_propierty_to_print        (Properties::minimum            );
    experiment.set_trialsSet_propierty_to_print        (Properties::median             );
    experiment.set_trialsSet_propierty_to_print        (Properties::_99                );
    experiment.set_trialsSet_propierty_to_print        (Properties::maximum            );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTime             );
    experiment.set_trialsSet_propierty_to_print        (Properties::repetition         );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTimePercentage   );
    //.

    //.
    //.
    
    //Writing results
    //experiment.writeResultsTo_stdcout          (false);
    experiment.writeResultsTo_file_makeRformat ();
    experiment.write_toFile_table_latexTabularFormat(false);
    //.
}

void Laboratory::audioTest() {
    
    Experiment experiment( "audioTest", resultsPath_ );
    
    //Making experiment
    {
        //Set general parameters
        InputAudioParameters str;
        str.data_source = AudioStream::AUDIO_FILE_AS_SOURCE;
        //.
        
        for(str.blockSize = /*MINIMUM BLOCK SIZE*/16; str.blockSize <= /*MAX BLOCK SIZE*/2048; str.blockSize *= 2)
        {
            std::stringstream s; s << experiment.title_ << std::string("__resultAudioFile_") << "blockSize-" << str.blockSize << ".wav";
            str.outFileName = s.str();
            experiment.add_trialsSet(      oneBufferForAllChannels_testFunction (str, experiment.title_)       );
        }
    }
    //.
    
    //Setting properties
    //Add
    experiment.properties_.add                         (Properties::experiment_title, std::string(PROPERTIE_VALUE__EXPERIMENT_TITLE__AUDIO_TEST));
    //.
    //Main
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::work_name               );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::ganged_or_multi_buffer  );
    experiment.set_trialsSet_propierty_as_mainProperty (Properties::lengthInSeconds         );
    //.
    //Print
    experiment.set_trialsSet_propierty_to_print        (Properties::repetition              );
    experiment.set_trialsSet_propierty_to_print        (Properties::num_of_channels         );
    experiment.set_trialsSet_propierty_to_print        (Properties::block_size              );
    //-
    experiment.set_trialsSet_propierty_to_print        (Properties::median                  );
    experiment.set_trialsSet_propierty_to_print        (Properties::_99                     );
    experiment.set_trialsSet_propierty_to_print        (Properties::repetition              );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTime                  );
    experiment.set_trialsSet_propierty_to_print        (Properties::onTimePercentage        );
    //.
    
    //.
    //.
    
    //Writing results
    experiment.writeResultsTo_stdcout          (false);
    experiment.writeResultsTo_file_makeRformat ();
    experiment.write_toFile_table_latexTabularFormat();
    //.
}










#pragma mark Test functions

TrialsSet Laboratory::FMMain_testFunction ( InputAudioParameters& str, InputFMParameters& fm_str, std::string folderForAudioResults, bool ifCutNotOnTime )
{
    using namespace std;
    
    //Creation
    FilterModule filterModule;
    filterModule.setParameters( fm_str );
    AudioStream audioStream( str.data_source, str.blockSize, &str.inFileName, &str.virtual_numOfChannels, &str.virtual_numOfBlocks );
    Timer timer;
    TrialsSet trialsSet;
    //.

    //Process
    processLoop<float_type>(filterModule, audioStream, timer, trialsSet);
    //.
    
    //Prepare result TrialsSet<double>
    addStandardAndStatisticsProperties(trialsSet, audioStream, str);
    //---
    trialsSet.properties_.add( Properties::impulse_response_N,          (unsigned int)  filterModule.getIRLength()      );
    trialsSet.properties_.add( Properties::impulse_response_C,          (unsigned int)  filterModule.getIRNumOfChann()  );
    trialsSet.properties_.add( Properties::convolutionerType,           (std::string)   FMParamsInfo::getString(fm_str.params_.operator[]<FMParamsInfo::convolutionerIDType>(FMParamsInfo::convolutionerID))    );
    trialsSet.properties_.add( Properties::num_mainKernel_LoopIteration,(unsigned int)  filterModule.getIRLength()/audioStream.get_blockSize()                                                                  );
    //---
    //GPU properties
    if(fm_str.params_.operator[]<FMParamsInfo::convolutionerIDType>(FMParamsInfo::convolutionerID) == FMParamsInfo::OPENCL__FREQUENCY_DOMAIN__UNIFORM_PARTITIONED) {
        trialsSet.properties_.add( Properties::gpuThreadsNumber,            (unsigned int)  audioStream.get_blockSize() * audioStream.get_numOfChannels() * 2                                                       );
        trialsSet.properties_.add( Properties::allocatedGPUMemory,          (double)  (8 * sizeof(float_type) * audioStream.get_numOfChannels() * (audioStream.get_blockSize() + filterModule.getIRLength()))/1024/1024 );
    }
    //.
    
    //Check
    checkAndSave(trialsSet, audioStream, str, ifOutSilence, folderForAudioResults);
    //.
    
    return trialsSet;
}

TrialsSet Laboratory::bufferForEachChannel_testFunction ( InputAudioParameters& str, std::string folderForAudioResults )
{
    using namespace std;
    
    //Creation
    CopyingProcessor_OpenCL proc;
    AudioStream audioStream( str.data_source, str.blockSize, &str.inFileName, &str.virtual_numOfChannels, &str.virtual_numOfBlocks );
    Timer timer;
    TrialsSet trialsSet;
    //.
    
    //Process
    processLoop<float>(proc, audioStream, timer, trialsSet);
    //.
    
    
    
    //Prepare result TrialsSet<double>
    addStandardAndStatisticsProperties(trialsSet, audioStream, str);
    //---
    trialsSet.properties_.add( Properties::ganged_or_multi_buffer,        std::string   (PROPERTIE_VALUE__GANGED_OR_MULTI_BUFFER__MULTI)                        );
    trialsSet.properties_.add( Properties::work_name,                     std::string   (PROPERTIE_VALUE__WORK_NAME__SIMPLY_DSP)  );
    //.
    
    //Check
    checkAndSave(trialsSet, audioStream, str, ifTheInTheSameAsOut, folderForAudioResults);
    //.
        
    return trialsSet;
}

TrialsSet Laboratory::oneBufferForAllChannels_testFunction ( InputAudioParameters& str, std::string folderForAudioResults )
{
    using namespace std;
    
    //Creation
    CopyingProcessor_Ganged_OpenCL proc;
    AudioStream audioStream( str.data_source, str.blockSize, &str.inFileName, &str.virtual_numOfChannels, &str.virtual_numOfBlocks );
    Timer timer;
    TrialsSet trialsSet;
    //.
    
    //Process
    processLoop<float>(proc, audioStream, timer, trialsSet);
    //.
    
    //Prepare result TrialsSet<double>
    addStandardAndStatisticsProperties(trialsSet, audioStream, str);
    //---
    trialsSet.properties_.add( Properties::ganged_or_multi_buffer,        std::string   (PROPERTIE_VALUE__GANGED_OR_MULTI_BUFFER__GANGED)                      );
    trialsSet.properties_.add( Properties::work_name,                     std::string   (PROPERTIE_VALUE__WORK_NAME__SIMPLY_DSP) );
    //.
    
    //Check
    checkAndSave(trialsSet, audioStream, str, ifTheInTheSameAsOut, folderForAudioResults);
    //.

    
    return trialsSet;
}















