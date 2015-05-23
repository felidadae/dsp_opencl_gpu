//
//  AudioFileIOManager.h
//  tester2
//
//  Created by kadlubek47 on 18.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __tester2__AudioFileIOManager_libsndfile__
#define __tester2__AudioFileIOManager_libsndfile__

#include <stdio.h>
#include <sndfile.h>
//---
#include "GeneralExceptions.h"


//float and double supported as SampleType
template<class SampleType>
class AudioFileIOManager {
public:
    
    class InStruct {
    public:
        long long int   channelLength;         /*out*/
        int             numOfChannels;         /*out*/
        unsigned int    sampleRate;            /*out*/
    };
    
    void read  ( SampleType** channels /*out*/,  const char* filePath , InStruct& str ) {
        
        //read into interleaved buffer
        recentlyReadFile_sfInfo_.format = 0;
        SNDFILE* inFile = sf_open (filePath, SFM_READ, &recentlyReadFile_sfInfo_);
        long long allLength = recentlyReadFile_sfInfo_.frames * recentlyReadFile_sfInfo_.channels;
        SampleType* buffer = new SampleType [allLength];
        
        switch (sizeof(SampleType)) {
            case sizeof(float):
                sf_read_float (inFile, (float*)buffer, allLength);
                break;
                
            case sizeof(double):
                sf_read_double(inFile, (double*)buffer, allLength);
                break;
            default:
                throw TemplateDoesNotSupportThatType_Exception();
                break;
        }
        //.
        
        *channels =  new SampleType [allLength];
        unsigned int channLength = (unsigned int) recentlyReadFile_sfInfo_.frames;
        unsigned int numOfChann  = (unsigned int) recentlyReadFile_sfInfo_.channels;
        
        //convert from interleaved to coalescing channels
        for(unsigned int channNum = 0; channNum < numOfChann; ++channNum)
            for(unsigned int sampleNum = 0; sampleNum < channLength; ++sampleNum)
                (*channels)[channNum*channLength + sampleNum] = buffer[ channNum%numOfChann + sampleNum*numOfChann ];
        //.
        
        //results
        str.numOfChannels  = numOfChann;
        str.sampleRate     = recentlyReadFile_sfInfo_.samplerate;
        str.channelLength  = channLength;
        //.
    }
    void write ( SampleType*  channels,          const char* filePath ) {
        SF_INFO newfile_sfInfo = recentlyReadFile_sfInfo_;
        SNDFILE* outFile = sf_open (filePath, SFM_WRITE, &newfile_sfInfo);
        
        long long blockSize = recentlyReadFile_sfInfo_.frames * recentlyReadFile_sfInfo_.channels;
        SampleType* buffer = new SampleType [blockSize];
        
        //convert from coalescing channels to interleaved channels
        for(unsigned int channNum = 0; channNum < recentlyReadFile_sfInfo_.channels; ++channNum)
            for(unsigned int sampleNum = 0; sampleNum < recentlyReadFile_sfInfo_.frames; ++sampleNum)
                buffer[channNum%recentlyReadFile_sfInfo_.channels + sampleNum*recentlyReadFile_sfInfo_.channels] = channels[ channNum*recentlyReadFile_sfInfo_.frames + sampleNum ];
        //.
        
        switch (sizeof(SampleType)) {
            case sizeof(float):
                sf_write_float(outFile, (float*)buffer, blockSize);
                break;
                
            case sizeof(double):
                sf_write_double(outFile, (double*)buffer, blockSize);
                break;
            default:
                throw TemplateDoesNotSupportThatType_Exception();
                break;
        }
        
        sf_close(outFile);
    }
    
private:
    SF_INFO recentlyReadFile_sfInfo_;
};



#endif /* defined(__tester2__AudioFileIOManager_libsndfile__) */
