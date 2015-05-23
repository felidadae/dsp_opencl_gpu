//
//  AudioStream.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 21.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __FilterModule_Standalone__AudioStream__
#define __FilterModule_Standalone__AudioStream__

#include <iostream>
#include "FMTypes.h"
#include "AudioFileIOManager.h"
#include "AudioInOutBuffers.h"
#include "TrialsSet.h"



class AudioStream {
public:
    enum DataSourceType {
        AUDIO_FILE_AS_SOURCE,
        RANDOM_DATA_AS_SOURCE
    };
    
    AudioInOutBuffers<float_type> channels_blocks_;
    
    AudioStream (DataSourceType dataSource, unsigned int blockSize, std::string* fileName = nullptr, unsigned int* numOfChannels = nullptr, unsigned int* repetition = nullptr );
    ~AudioStream();
    
    bool firstBlockTest();
    bool nextBlock();
    
    void normalizeMax();
    void close      (std::string fileName, bool ifNormalize = true);
    void cutAndClose(std::string fileName, TrialsSet& trialsSet, bool ifNormalize = false );

    
    bool checkIfOutSilence();
    bool checkIfOutTheSameAsIn();
    
    //get
    unsigned int get_blockSize()    { return channels_blocks_.channelLength_; }
    unsigned int get_sampleRate()   { return sampleRate_; }
    unsigned int get_numOfChannels(){ return channels_.numOfChannels_; }
    unsigned int get_numOfBlocks()  { return numOfBlocks_; }
    //.
    
private:
    AudioInOutBuffers<float_type> channels_;
    unsigned int sampleRate_  = 0;
    unsigned int    i_block_  = 0;
    unsigned int numOfBlocks_ = 0;
    
    DataSourceType currentDataSource_;  //If currentDataSource_==RANDOM_DATA_AS_SOURCE then channels_ are not used,
                                        //due to possibiliti of allocating too much memory (128channels 1024 samples per channel,
                                        //5000 blocks)
    
    bool ifSilent_ = true, ifTheSameAsIn_ = true; //if true to current block
    
    AudioFileIOManager<float_type> fileIOManager_;
};



#endif /* defined(__FilterModule_Standalone__AudioStream__) */
