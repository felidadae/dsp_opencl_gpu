//
//  Laboratory.h
//  GPUProcessingTesting
//
//  Created by kadlubek47 on 12.07.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __GPUProcessingTesting__Laboratory__
#define __GPUProcessingTesting__Laboratory__

#include <string>
//---
#include "TrialsSet.h"
#include "Experiment.h"
//---
#include "FMParamsInfo.h"
#include "FMTypes.h"
#include "AudioStream.h"
#include "IProcess.h"
#include "IParameterizable.h"
#include "AudioStream.h"



//Filter Module Laboratory
class Laboratory {
public:
    
    // Constructing
    Laboratory();
    void startComputations(bool ifCarryExperiments, unsigned int blockSize, std::string hardware);

    // Experiments
    void                 FMAudioTest(); 
    void                FMComparison();
    void                FMGPUGeneral();
    void                       FMMax(FMParamsInfo::convolutionerIDType convolutionerID);
    //---
    void                   audioTest();
    void      multiChannelExperiment();
    void minimumBufferSizeExperiment();
    
    //Struct in
    class InputAudioParameters;
    class InputFMParameters;

    // Test functions
    TrialsSet                  FMMain_testFunction ( InputAudioParameters& str, InputFMParameters& fm_str, std::string folderForAudioResults, bool ifCutNotOnTime = true );
    //---
    TrialsSet    bufferForEachChannel_testFunction ( InputAudioParameters& str, std::string folderForAudioResults );
    TrialsSet oneBufferForAllChannels_testFunction ( InputAudioParameters& str, std::string folderForAudioResults );
    
private:
    std::string resultsPath_;
    
    static const unsigned int defaultSampleRate_ = 44100;
    
    template <class SampleType>
    void processLoop(IProcess<SampleType>& processObject, AudioStream& audioStream, Timer& timer, TrialsSet& trialsSet) {
        
        //Prepare to processing..
        audioStream.firstBlockTest();
        processObject.process( audioStream.channels_blocks_ );
        processObject.reset();
        //.
        
        //Processing
        timer.start(Timer::MAIN_TIMER);
        do {
            timer.start(Timer::TRIAL_TIMER);
            processObject.process( audioStream.channels_blocks_ );
            timer.stop (Timer::TRIAL_TIMER);
        } while( audioStream.nextBlock() );
        timer.stop(Timer::MAIN_TIMER);
        //.
        
        
        trialsSet.add_trials    ( timer.get_trialsTimings() );
        trialsSet.add_mainTime  ( timer.get_mainTiming()    );
    }
    
    //for test functions
    void addStandardAndStatisticsProperties(TrialsSet& trialsSet, AudioStream& audioStream, InputAudioParameters& str);
    enum CheckMethodType {
        ifTheInTheSameAsOut,
        ifOutSilence
    };
    void checkAndSave (TrialsSet& trialsSet, AudioStream& audioStream, InputAudioParameters& str, CheckMethodType checkMethod, std::string folderForAudioResults );
};



class Laboratory::InputAudioParameters {
public:
    
    double lengthInSeconds = 30;
    unsigned int sampleRate  = 44100;
    unsigned int blockSize   = 1024; //< latency
    
    //Data Source
    AudioStream::DataSourceType data_source = AudioStream::AUDIO_FILE_AS_SOURCE;
                                            //AudioStream::RANDOM_DATA_AS_SOURCE;
    //.
    
    //Testing FROM_AUDIO_FILE
    static std::string  inFileName;
    std::string outFileName  = std::string("resultFile.wav");
    //.
    
    //Testing RANDOM_DATA_AS_SOURCE
    unsigned int virtual_numOfChannels      = 15;       /*num of channels of input audio and ir*/
    unsigned int virtual_numOfBlocks        = 1000;     /*repetition*/
    unsigned int virtual_irN                = 250000;   /*num of samples per ir channel*/
    //.
    
};



class  Laboratory::InputFMParameters {
public:
    
    static std::string irFilePath;
    
    InputFMParameters( Laboratory::InputAudioParameters& str ) {
        FMParamsDefVal::setDefault(params_);
        params_.add(FMParamsInfo::irFilePath, irFilePath );
        update(str);
    }
    
    void update( Laboratory::InputAudioParameters& str ) {
        if(str.data_source == AudioStream::RANDOM_DATA_AS_SOURCE) {
            params_.add(FMParamsInfo::irfcID, FMParamsInfo::TEST);
            params_.add(FMParamsInfo::length, str.virtual_irN);
            params_.add(FMParamsInfo::numOfChannels, str.virtual_numOfChannels);
        }
    }
    
    operator FMParameters () const { return params_; }
    
    FMParameters params_;
};



#endif /* defined(__GPUProcessingTesting__Laboratory__) */
