//
//  AudioStream.cpp
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 21.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "AudioStream.h"



#pragma mark Construct, destroy

AudioStream::AudioStream(DataSourceType dataSource, unsigned int blockSize, std::string* fileName, unsigned int* numOfChannels, unsigned int* repetition) {
    
    channels_blocks_.channelLength_ = blockSize;
    currentDataSource_ = dataSource;
    
    if(dataSource == AUDIO_FILE_AS_SOURCE)
    {
        //Read file..
        AudioFileIOManager<float_type>::InStruct str;
        fileIOManager_.read( (&channels_.in_.data_), fileName->c_str(), str );
        //.
        
        //Initialize channels_, channels_blocks_
        //Assignments
        numOfBlocks_                    = (unsigned int) (str.channelLength / (long long) blockSize);
        channels_.numOfChannels_        = channels_blocks_.numOfChannels_ = str.numOfChannels;
        channels_.channelLength_        = (unsigned int) str.channelLength;
        sampleRate_ = str.sampleRate;
        //.
        
        //create buffers
        channels_.out_         = new float_type [       channels_.channelLength_ *        channels_.numOfChannels_];
        for(unsigned int i = 0; i < channels_.channelLength_ * channels_.numOfChannels_; ++i)
            channels_.out_.data_[i] = 0;
        channels_blocks_.in_   = new float_type [channels_blocks_.channelLength_ * channels_blocks_.numOfChannels_];
        channels_blocks_.out_  = new float_type [channels_blocks_.channelLength_ * channels_blocks_.numOfChannels_];
        //.
        //.
        
        //prepare first block
        for(unsigned int channNum = 0; channNum < channels_blocks_.numOfChannels_; ++channNum)
            for(unsigned int sampleNum = 0; sampleNum < channels_blocks_.channelLength_; ++sampleNum)
                channels_blocks_.in_[channNum][sampleNum] = channels_.in_[channNum][sampleNum + i_block_*channels_blocks_.channelLength_];
        //.

    }
    
    if(dataSource == RANDOM_DATA_AS_SOURCE) {
        //Assignments
        numOfBlocks_                    = *repetition;
        channels_.numOfChannels_        = *numOfChannels;//..
        channels_blocks_.numOfChannels_ = *numOfChannels;//..
        channels_.channelLength_        = (*repetition) * blockSize;
        sampleRate_ = 44100;
        //.
        
        //create buffers
        channels_blocks_.in_   = new float_type [channels_blocks_.channelLength_ * channels_blocks_.numOfChannels_];
        channels_blocks_.out_  = new float_type [channels_blocks_.channelLength_ * channels_blocks_.numOfChannels_];//.
        //init
        srand((unsigned int)(time(NULL)));
        for(unsigned int i = 0; i < channels_blocks_.channelLength_ * channels_blocks_.numOfChannels_; ++i)
            channels_blocks_.in_.data_[i] = ((float_type)(rand()%300))/300.0f;
        //.
    }
    
}

AudioStream::~AudioStream() {}



#pragma mark Actions

bool AudioStream::firstBlockTest() {
    if(numOfBlocks_ == 0 || i_block_ == numOfBlocks_)
        return false;
    if(i_block_ == numOfBlocks_)
        return false;
    
    if(currentDataSource_ == AUDIO_FILE_AS_SOURCE) {
        //write into block_channelsIn next block;
        for(unsigned int channNum = 0; channNum < channels_blocks_.numOfChannels_; ++channNum)
            for(unsigned int sampleNum = 0; sampleNum < channels_blocks_.channelLength_; ++sampleNum)
                channels_blocks_.in_[channNum][sampleNum] = channels_.in_[channNum][sampleNum + i_block_ * channels_blocks_.channelLength_];
    }
    
    if(currentDataSource_ == RANDOM_DATA_AS_SOURCE) {
        for(unsigned int i = 0; i < channels_blocks_.channelLength_ * channels_blocks_.numOfChannels_; ++i)
            channels_blocks_.out_.data_[i] = 0;
    }
    
    return true;
}

bool AudioStream::nextBlock() {
    
    if(numOfBlocks_ == 0 || i_block_ == numOfBlocks_)
        return false;
    
    //check if already processed block is silent block and if out the same as in
    for(unsigned int i = 0; i < channels_blocks_.channelLength_ * channels_blocks_.numOfChannels_; ++i) {
        if (channels_blocks_.out_.data_[i] != 0)
            ifSilent_ = false;
        if (channels_blocks_.out_.data_[i] != channels_blocks_.in_.data_[i])
            ifTheSameAsIn_ = false;
    }
    //.
    
    //Do channels_.out_ channels_blocks_.out_
    if(currentDataSource_ == AUDIO_FILE_AS_SOURCE) {
        //read
        for(unsigned int channNum = 0; channNum < channels_blocks_.numOfChannels_; ++channNum)
            for(unsigned int sampleNum = 0; sampleNum < channels_blocks_.channelLength_; ++sampleNum)
                channels_.out_[channNum][sampleNum + i_block_*channels_blocks_.channelLength_] = channels_blocks_.out_[channNum][sampleNum];
        //.
    }
    if(currentDataSource_ == RANDOM_DATA_AS_SOURCE) {
        //set to zero
        for(unsigned int i = 0; i < channels_blocks_.channelLength_ * channels_blocks_.numOfChannels_; ++i)
            channels_blocks_.out_.data_[i] = 0;
        //.
    }
    //.
    
    
    //Increment block index
    ++i_block_;
    if(i_block_ == numOfBlocks_)
        return false;
    //.
    
    
    //Prepera channels_blocks_.in_
    if(currentDataSource_ == AUDIO_FILE_AS_SOURCE) {
        //write into block_channelsIn next block;
        for(unsigned int channNum = 0; channNum < channels_blocks_.numOfChannels_; ++channNum)
            for(unsigned int sampleNum = 0; sampleNum < channels_blocks_.channelLength_; ++sampleNum)
                channels_blocks_.in_[channNum][sampleNum] = channels_.in_[channNum][sampleNum + i_block_ * channels_blocks_.channelLength_];
    }
    //.
    
    return true;
}

void AudioStream::normalizeMax() {
    
    if(currentDataSource_ != AUDIO_FILE_AS_SOURCE)
        return;
    
    //normalize
    //(first channel sum)
    float_type max_in = abs (channels_.in_[0][0]);
    for(int i = 1; i < channels_.channelLength_; ++i) {
        float_type temp;
        if(channels_.in_[0][i] < 0)
            temp = 0 - channels_.in_[0][i];
        else
            temp = channels_.in_[0][i];
        if(temp > max_in)
            max_in = temp;
    }
    
    float_type max_out = abs (channels_.out_[0][0]);
    for(int i = 1; i < channels_.channelLength_; ++i) {
        float_type temp;
        if(channels_.out_[0][i] < 0)
            temp = 0 - channels_.out_[0][i];
        else
            temp = channels_.out_[0][i];
        if(temp > max_out)
            max_out = temp;
    }
    
    float_type normalization = max_out/max_in;
    for(unsigned int i = 0; i < channels_.out_.get_allLength(); ++i )
        channels_.out_.data_[i] = channels_.out_.data_[i] / normalization;
    //.

}

void AudioStream::close (std::string fileName, bool ifNormalize) {
    if(currentDataSource_ != AUDIO_FILE_AS_SOURCE)
        return;
    
    if(ifNormalize)
        normalizeMax();
    fileIOManager_.write( channels_.out_.data_, fileName.c_str() );
}

void AudioStream::cutAndClose (std::string fileName, TrialsSet& trialsSet, bool ifNormalize ) {
    if(currentDataSource_ != AUDIO_FILE_AS_SOURCE)
        return;
    
    if(ifNormalize)
        normalizeMax();
    
    unsigned int blockSize      = boost::any_cast<unsigned int> (     trialsSet.properties_.get(Properties::block_size).first           );
    unsigned int numOfChannels  = boost::any_cast<unsigned int> (     trialsSet.properties_.get(Properties::num_of_channels).first      );
    unsigned int sampleRate     = boost::any_cast<unsigned int> (     trialsSet.properties_.get(Properties::sample_rate).first          );
    
    double latencyValue = (double) blockSize /  (double) sampleRate * 1000 /*ms*/;
    
    size_t numOfTrials = trialsSet.get_trialsNumber();
    for(size_t trialNum = 0; trialNum < numOfTrials; ++trialNum )
        if( trialsSet.get_trial( trialNum ) > latencyValue ) {
            //clean that group of blocks to zeros..
            for(unsigned int channNum = 0; channNum < channels_.numOfChannels_; ++channNum)
                for(unsigned int sampleNum = 0; sampleNum < channels_blocks_.channelLength_; ++sampleNum)
                    channels_.out_[channNum][sampleNum + trialNum*channels_blocks_.channelLength_] = 0;
            //.
        }
    
    
    fileIOManager_.write( channels_.out_.data_, fileName.c_str() );
}

bool AudioStream::checkIfOutSilence() {
    return ifSilent_;
}

bool AudioStream::checkIfOutTheSameAsIn() {
    return ifTheSameAsIn_;
}