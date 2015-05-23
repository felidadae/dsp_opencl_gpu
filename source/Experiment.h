#ifndef __GPUProcessingTesting__Experiment__
#define __GPUProcessingTesting__Experiment__

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <boost/any.hpp>
#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#include "TrialsSet.h"
#include "Properties.h"
#include "Timer.h"


class Experiment {
public:
    template <class NumberType>
    static std::string getStringFromNumber(unsigned int precision, NumberType number) {
        std::stringstream s;
        s.precision(precision);
        s << std::fixed << number;
        return s.str();
    }
    
    //Create, destroy
    Experiment(const std::string& title, const std::string& folderPath);
    ~Experiment();
    //.
    
    
    
    //Writing result
    //Writing each trial set one after another - general way of showing result
    void writeResultsTo_file            (bool ifEachTrialPrinted = false);
    void writeResultsTo_stdcout         (bool ifEachTrialPrinted = false);
    void writeResultsTo_file_makeRformat(std::string addToName = std::string(""));
    void write_toFile_table_latexTabularFormat(bool printIRLength = false);   //Channels & Block size & Latency & Blocks on time & Avg  & ...
    //.
    
    //Specific for comparison and others - should be put in different class, because of it is not general
    void write_toFile_table_latexTabularFormat_comparison();    //IR lenghth & Block size & CPU Blocks on time & GPU Blocks on time & avg/avg
    void write_toFile_table_latexTabularFormat_Max(bool ifGPUComputation);           //blockSize & MaxChannels & ThreadsNumber & TransformSize & Loop
    //.
    
    
    
    
    Properties properties_; /*common for all trialsSets*/

    void add_trialsSet                              (TrialsSet trialsSet) { trialsSets_.push_back(trialsSet); }

    void set_trialsSet_propierty_to_print           (std::string title) {
        trialsSet_propierties_printed_.push_back(title);
    }
    void set_trialsSet_propierties_to_print         (std::vector<std::string> titles) {
        foreach (std::string& s, titles)
            set_trialsSet_propierty_to_print(s);
    }

    void set_trialsSet_propierty_as_mainProperty    (std::string title) {
        trialsSet_propierty_set_as_mainProperty_.push_back(title);
    }
    void set_trialsSet_propierties_as_mainProperty (std::vector<std::string> titles) {
        foreach (std::string& s, titles)
            set_trialsSet_propierty_as_mainProperty(s);
    }

    const std::string folderPath_, title_;
    
private:
    std::vector< TrialsSet >    trialsSets_;
    std::vector< std::string >  trialsSet_propierties_printed_;
    std::vector< std::string >  trialsSet_propierty_set_as_mainProperty_;
    
    static const std::string after_main_properties__mark_, after_trialsSet_properties__mark_, before_trialsSet_properties__mark_;
    void writeToStream  (std::ostream& outputStream, bool ifEachTrialPrinted = true);
};


#undef foreach

#endif /* defined(__GPUProcessingTesting__Experiment__) */