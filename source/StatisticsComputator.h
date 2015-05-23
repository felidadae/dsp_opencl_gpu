//
//  StatisticsComputator.h
//  DSPOpenCLGPUTester
//
//  Created by kadlubek47 on 10.01.2014.
//  Copyright (c) 2014 kadlubek47. All rights reserved.
//

#ifndef DSPOpenCLGPUTester_StatisticsComputator_h
#define DSPOpenCLGPUTester_StatisticsComputator_h

#include <algorithm>    // std::sort
#include <sstream>
#include "TrialsSet.h"


class StatisticsComputator {
public:
    class Statistics {
    public:
        double average;
        double averagePerSample;
        //---
        double blocksOnTime;
        double allBlocks;
        double percentageBlocksOnTime;
        //---
        double minimum;
        double median;
        double _99procent;
        double maximum;
        //---
    };
    static void compute        (TrialsSet& trialsSet_) {
        Statistics result;
        size_t numOfTrials = result.allBlocks = trialsSet_.get_trialsNumber();
        std::vector<double> sortedTrials = trialsSet_.get_trials();
        std::sort ( sortedTrials.begin(), sortedTrials.end() );
        //.
        
        //averageTrialValue
        double sum = 0;
        for(size_t trialNum = 0; trialNum < numOfTrials; ++trialNum )
            sum += trialsSet_.get_trial( trialNum );
        
        result.average = sum / (double)numOfTrials;
        //.
        
        //minimal, median, _99, maximum
        unsigned int _99index = numOfTrials*0.99;
        result.minimum      = sortedTrials[0];
        result.median       = sortedTrials[numOfTrials/2];
        result._99procent   = sortedTrials[_99index-1];
        result.maximum      = sortedTrials[numOfTrials-1];
        //.
        
        //numOfBlocksOnTime
        unsigned int blockSize      = boost::any_cast<unsigned int> (     trialsSet_.properties_.get(Properties::block_size).first           );
        unsigned int numOfChannels  = boost::any_cast<unsigned int> (     trialsSet_.properties_.get(Properties::num_of_channels).first      );
        unsigned int sampleRate     = boost::any_cast<unsigned int> (     trialsSet_.properties_.get(Properties::sample_rate).first          );
        
        double latencyValue = (double) blockSize /  (double) sampleRate * 1000 /*ms*/;
        result.blocksOnTime = 0;
        for(size_t trialNum = 0; trialNum < numOfTrials; ++trialNum )
            if( trialsSet_.get_trial( trialNum ) < latencyValue )
                ++result.blocksOnTime;
        
        result.percentageBlocksOnTime = (double)result.blocksOnTime / (double)numOfTrials * 100 ;
        result.averagePerSample       = result.average / ((double) blockSize) / ((double) numOfChannels) * 1000;
        //.
        
        //add properties
        trialsSet_.properties_.add              (Properties::averageTimePerSample, result.averagePerSample          );
        trialsSet_.properties_.add              (Properties::average             , result.average                   );
        trialsSet_.properties_.add              (Properties::minimum             , result.minimum                   );
        trialsSet_.properties_.add              (Properties::median              , result.median                    );
        trialsSet_.properties_.add              (Properties::_99                 , result._99procent                );
        trialsSet_.properties_.add              (Properties::maximum             , result.maximum                   );
        trialsSet_.properties_.add              (Properties::onTime              , result.blocksOnTime              );
        trialsSet_.properties_.add              (Properties::onTimePercentage    , result.percentageBlocksOnTime    );
        //.
        
    }
};

#endif
