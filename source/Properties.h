//
//  Properties.h
//  FilterModule_Standalone
//
//  Created by kadlubek47 on 21.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#ifndef FilterModule_Standalone_Description_h
#define FilterModule_Standalone_Description_h

#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <boost/any.hpp>
#include <sstream>


class Properties {
public:
    
    //experiment
    static const std::string experiment_title;              //std::string
    //.
    
    //general
    static const std::string lengthInSeconds;               //unsigned int
    static const std::string repetition;                    //unsigned int
    static const std::string block_size;                    //unsigned int
    static const std::string sample_rate;                   //unsigned int
    static const std::string num_of_channels;               //unsigned int
    static const std::string ganged_or_multi_buffer;        //std::string
    static const std::string work_name;                     //std::string
    static const std::string latency;                       //double
    //.
    
    //gpu
    static const std::string allocatedGPUMemory;            //double  (MB)
    static const std::string gpuThreadsNumber;              //unsigned int
    //.
    
    //statistics
    static const std::string averageTimePerSample;          //double
    static const std::string average;                       //double
    static const std::string minimum;                       //double
    static const std::string median;                        //double
    static const std::string _99;                           //double
    static const std::string maximum;                       //double
    static const std::string onTime;                        //unsigned int
    static const std::string onTimePercentage;              //double
    //
    
    //filtering
    static const std::string impulse_response_N;            //unsigned int
    static const std::string impulse_response_C;            //unsigned int
    static const std::string convolutionerType;             //std::string
    static const std::string num_mainKernel_LoopIteration;  //unsigned int
    //.
    
    
    
    //_____________________________________________________________________
    
    //add new element
    template < class AddedClass/*must be able to print by std::cout << */ >
    void add(std::string title, AddedClass a ) {
        using namespace std;
        std::stringstream str_stream;
        str_stream << a;
        properties_.insert( pair<   string, pair<boost::any,string>   > (title, pair<boost::any,string> (boost::any(a), str_stream.str()) ));
    }
    //.
    
    //add user struct
    template < class AddedClass/*must be able to print by std::cout << */ >
    void add_user_struct(std::string title, AddedClass a ) {
        using namespace std;
        std::stringstream str_stream;
        str_stream << std::string(a);
        properties_.insert( pair<   string, pair<boost::any,string>   > (title, pair<boost::any,string> (boost::any(a), str_stream.str()) ));
    }
    //.
    
    //get element from title
    std::pair<boost::any, std::string> get(std::string title)  {
        if(properties_.find(title) == properties_.end())
            throw int();
        return std::pair<boost::any, std::string> ( properties_.find(title)->second );
    }
    std::map <std::string, std::pair<boost::any, std::string>  >    get_allProperties()     { return properties_;}
    std::string getStringFromFloatWithPrecision ( std::string title, unsigned int precision ) {
        if(properties_.find(title) == properties_.end())
            throw int();
        
        std::stringstream s;
        s.precision(precision);
        s << std::fixed << boost::any_cast<double>(properties_.find(title)->second.first);
        return s.str();
    }
    //.
    
private:
    std::map <std::string, std::pair<boost::any, std::string>  > properties_;
};

#endif
