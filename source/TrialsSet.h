//
//  TrialsSet.h
//  GPUProcessingTesting
//
//  Created by kadlubek47 on 10.07.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef __GPUProcessingTesting__TrialsSet__
#define __GPUProcessingTesting__TrialsSet__

//includes
#include <iostream>
#include <map>
#include <vector>
#include <boost/any.hpp>
//.

#include "Properties.h"


class TrialsSet {
public:
    //add
    void add_mainTime(double mainTime) { mainTime_ = mainTime; }
    void add_trials(std::vector<double> trials) { trials_ = trials; }
    //.
    
    //get
    std::vector<double>                 get_trials() { return trials_; }
    double                              get_mainTime() {return mainTime_;}
    double                              get_trial(size_t index)   {return trials_[index];}
    size_t                              get_trialsNumber()  {return trials_.size();}
    //.
    
    Properties properties_;
    
private:
    double mainTime_;
    std::vector<double> trials_;
};


/*
class Linear  {
public:
    double _0                   = -1;
    double _50                  = -1;
    double _99procent           = -1;
    double maximum              = -1;
 
    operator std::string() { std::stringstream s; s << "( " << minimum << ", " << quartile1 << ", " << median << ", " << quartile3 << ", " << maximum << ")"; return s.str(); }
    std::string getLatexTabular() {
        std::stringstream s;
        s.precision(2);
        s << std::fixed << "$" << minimum << "$ & $" << quartile1 << "$ & $" << median << "$ & $" << quartile3 << "$ & $" << maximum << "$"; return s.str();
    }
    
};
class Average {
public:
    double average              = -1;
    double averagePerSample     = -1;
    
    operator std::string() { std::stringstream s; s << "( " << average <<  ")"; return s.str(); }
    std::string getLatexTabular() {
        std::stringstream s;
        s.precision(2);
        s << std::fixed << "$" <<  average << "$"; return s.str(); }
};
class OnTime  {
public:
    double all;
    double onTime               = -1;
    double onTime_percentage    = -1;
    
    operator std::string() { std::stringstream s; s << "(" << onTime << "/" << all << ", " << onTime_percentage << "%)"; return s.str(); }
    std::string getLatexTabular() {
        std::stringstream s;
        
        s << "$" << onTime << "/" << all << "$, $";
        s.precision(4);
        s << std::fixed << onTime_percentage << "\\\%$"; return s.str();
    }
};

Linear result;
Average result_average;
OnTime result_ontime;
Latency result_latency;*/




#endif /* defined(__GPUProcessingTesting__TrialsSet__) */
