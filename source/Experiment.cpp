//
//  Experiment.cpp
//  GPUProcessingTesting
//
//  Created by kadlubek47 on 20.08.2013.
//  Copyright (c) 2013 kadlubek47. All rights reserved.
//

#include "Experiment.h"
#include <sys/stat.h>


const std::string Experiment::after_main_properties__mark_           = std::string ("-------------------\n");
const std::string Experiment::after_trialsSet_properties__mark_      = std::string ("***\n");
const std::string Experiment::before_trialsSet_properties__mark_     = std::string ("\n\n\n");



#pragma mark Construct, destroy

Experiment::Experiment(const std::string& title, const std::string& folderPath): title_(title), folderPath_(folderPath) {
    mkdir((folderPath_+title_).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
}

Experiment::~Experiment() {}



#pragma mark Writing to file general

void Experiment::writeResultsTo_file   (bool ifEachTrialPrinted) {
    using namespace std;
    
    //Creating file for writing
    string filePath;
    filePath.append(folderPath_);
    filePath.append(title_);
    filePath.append("/generalRaport");
    filePath.append(".txt");
    ofstream fs(filePath.c_str(), ios_base::out | ios_base::trunc);
    //.
    writeToStream(fs);
    fs.close();
}

void Experiment::writeResultsTo_stdcout(bool ifEachTrialPrinted) {
    writeToStream(std::cout, false);
}

void Experiment::writeToStream(std::ostream& outputStream, bool ifTrialsArrayPrint) {

    //Main properties
    //main properties
    std::map <std::string, std::pair<boost::any, std::string>  > copy = properties_.get_allProperties();
    for(std::map <std::string, std::pair<boost::any, std::string>  >::iterator it = copy.begin(); it != copy.end(); ++it)
        outputStream <<  it->first << ": " << it->second.second << "\n";
    //.
    
    //trials set properies set as main
    for(std::vector<std::string>::iterator it = trialsSet_propierty_set_as_mainProperty_.begin(); it != trialsSet_propierty_set_as_mainProperty_.end(); ++it)
        outputStream << *it  << ": " << trialsSets_[0].properties_.get(*it).second << "\n";
    //.
    //.
    outputStream  <<  after_main_properties__mark_;
    
    for ( std::vector<TrialsSet>::iterator it_trialSet = trialsSets_.begin(); it_trialSet != trialsSets_.end(); ++it_trialSet ) {
        outputStream  <<  before_trialsSet_properties__mark_;
        //TrialsSet properties
        for(std::vector<std::string>::iterator it = trialsSet_propierties_printed_.begin(); it != trialsSet_propierties_printed_.end(); ++it)
            outputStream <<  *it << ": " << it_trialSet->properties_.get(std::string(*it)).second << "\n";
        //.
        
        //Trials array
        if ( ifTrialsArrayPrint ) {
            outputStream    <<  after_trialsSet_properties__mark_;
            for(unsigned int i_trial = 0; i_trial < it_trialSet->get_trialsNumber(); ++i_trial)
                outputStream <<      it_trialSet->get_trial(i_trial)      << "\n";
        }
        //.
    }
    
}

void Experiment::writeResultsTo_file_makeRformat(std::string addToName) {
    
    using namespace std;
    
    //Creating file for writing
    string filePath;
    filePath.append(folderPath_);
    filePath.append(title_);
    filePath.append("/generalReport");
    filePath.append(addToName);
    filePath.append(".r");
    ofstream outputStream(filePath.c_str(), ios_base::out | ios_base::trunc);
    //.
    
    //R
    outputStream << "setwd ( \"" << folderPath_ + title_ << "\")\n" //set working directory in R
                    "rm(list = ls())" << "\n";              //clear workspace (if there was some changes by different programs)
    //.
    
    //Main properties
    //main properties
    std::map <std::string, std::pair<boost::any, std::string>  > copy = properties_.get_allProperties();
    outputStream << "mainProperties <- \"";
    for(std::map <std::string, std::pair<boost::any, std::string>  >::iterator it = copy.begin(); it != copy.end(); ++it)
        outputStream <<  it->first << ": " << it->second.second << "\\n";
    //.
    //trials set properies set as main
    for(std::vector<std::string>::iterator it = trialsSet_propierty_set_as_mainProperty_.begin(); it != trialsSet_propierty_set_as_mainProperty_.end(); ++it)
        outputStream << *it  << ": " << trialsSets_[0].properties_.get(*it).second << "\\n";
    //.
    outputStream << "\"\n";
    //.
    
    outputStream << "trialsSets_properties <- vector( length = " << trialsSets_.size() << ")\n";
    outputStream << "trialsSets <- list()\n";
    int i = 0;
    for ( std::vector<TrialsSet>::iterator it_trialSet = trialsSets_.begin(); it_trialSet != trialsSets_.end(); ++i, ++it_trialSet ) {
        
        //TrialsSet properties
        outputStream << "trialsSets_properties[" << i+1 << "] <- \"";
        for(std::vector<std::string>::iterator it = trialsSet_propierties_printed_.begin(); it != trialsSet_propierties_printed_.end(); ++it)
            outputStream <<  *it << ": " << it_trialSet->properties_.get(std::string(*it)).second << "\\n";
        outputStream << "\"\n";
        //.
        
        //Trials array
        outputStream  <<  "trialsSets[["<< i+1 << "]] <- c( ";
        for(unsigned int i_trial = 0; i_trial < it_trialSet->get_trialsNumber(); ++i_trial) {
            outputStream <<     it_trialSet->get_trial(i_trial);
            if(i_trial != it_trialSet->get_trialsNumber()-1 )
                outputStream << ", ";
        }
        outputStream <<  ")\n";
        //.
    }
    
    outputStream << "\npdf(file=\"" << "generalRaport" << ".pdf\", encoding = \"ISOLatin2.enc\", width = 9, height = 8 )\n"
                    "library(gplots)\n"
                    "textplot(mainProperties,cex=1)\n"
                    "\n"
                    "par( mfrow = c(1,2), oma = c(13,0,1,0), font.axis = 3, font.lab = 3 )\n"
                    "for(i in 1:length(trialsSets)) {\n"
                        "\tplot ( trialsSets[[i]], main = \"\", xlab=\"Indeks repetycji\", ylab=\"Czas wykonania [ms]\" )\n"
                        "\thist ( trialsSets[[i]], breaks = 50, main = \"\", xlab=\"Czas wykonania [ms]\", ylab=\"Ilość wystąpień\" )\n"
                        "\tmtext( side=3, line = -2, \"Wyniki czasowe eksperymentu\", outer=TRUE, cex = 2 )\n"
                        "\tmtext( side=1, line = 12, trialsSets_properties[i], outer=TRUE, padj = 0, adj = 0, cex = 1)\n"
                    "}\n"
                    "dev.off()\n";
}

void Experiment::write_toFile_table_latexTabularFormat(bool printIRLength) {
    using namespace std;
    
    //Creating file for writing
    string filePath;
    filePath.append(folderPath_);
    filePath.append(title_);
    filePath.append("/generalTableReport");
    filePath.append(".tex");
    ofstream outputStream(filePath.c_str(), ios_base::out | ios_base::trunc);
    //.
    
    outputStream << "\\begin{table}\n"
                    "\\footnotesize\n"
                    "\\centering\n"
                    "\\begin{tabular}{c|c|c|r||c||c|c||c|c|c|c}\n"
                    "\\hline\n"
                    "N\\textsubscript{OI} & Kan & Blok & Lat & Ilość bloków na czas & Śred & ŚredS & Min & Med & 99\\% & Max \\\\ \n"
                    "\\hline\n";
    
    //
    std::string lastIRLength("");
    std::string lastNumOfChannels("");
    //---
    for (std::vector<TrialsSet>::iterator it_trialSet = trialsSets_.begin(); it_trialSet != trialsSets_.end(); ++it_trialSet) {
        bool printedLine=false;
        if(printIRLength && ((it_trialSet->properties_).get(Properties::impulse_response_N).second != lastIRLength) ) {
            outputStream << "\\hline\n";
            printedLine = true;
            outputStream << "$";
            outputStream << (it_trialSet->properties_).get(Properties::impulse_response_N).second;                          //IRLength
            outputStream << "$";
            lastIRLength = (it_trialSet->properties_).get(Properties::impulse_response_N).second;

        }
        
        if((it_trialSet->properties_).get(Properties::num_of_channels).second != lastNumOfChannels) {
            if(!printedLine) {
                if(!printIRLength) {
                    outputStream << "\\hline\n";
                }
                else
                    outputStream << "\t\\cline{2-11}\n";
            }
            outputStream << "\t& ";
            printedLine = false;
            outputStream << "$";
            outputStream << (it_trialSet->properties_).get(Properties::num_of_channels).second;                             //channelNumbers
            outputStream << "$";
            lastNumOfChannels = (it_trialSet->properties_).get(Properties::num_of_channels).second;
        }
        else
            outputStream << "\t& ";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).get(Properties::block_size).second;                                      //blockSize
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::latency, 2);                 //latency
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).get(Properties::onTime).second;                                          //blocks on time
        outputStream << "/";
        outputStream << (it_trialSet->properties_).get(Properties::repetition).second;
        outputStream << ", ";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::onTimePercentage, 4);
        outputStream << "\\%$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::average, 2);                 //average
        outputStream << "$";
        outputStream << "\t\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::averageTimePerSample, 2);    //averagePerSample
        outputStream << "$";
        outputStream << "\t\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::minimum, 2);                 //minimum
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::median, 2);                  //median
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::_99, 2);                     //99 procent
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::maximum, 2);                 //maximum
        outputStream << "$";
        outputStream << "\\\\ \n";
    }
    //.
    
    std::vector<TrialsSet>::iterator it_trialSet = trialsSets_.begin();
    std::string lengthOfAudioInSeconds = (it_trialSet->properties_).get(Properties::lengthInSeconds).second;
    std::string sampleRate = (it_trialSet->properties_).get(Properties::sample_rate).second;
    
    
    
    
    outputStream << "\\hline\n"
                    "\\end{tabular}\n"
                    "\\caption{ Wyniki czasowe. Czas trwania pliku wejściowego: " << lengthOfAudioInSeconds << "s; częstotliwość próbkowania: " << sampleRate << "$Hz$ }\n"
                    "\\label{table:TYTUL}\n"
                    "\\end{table}\n";
    
    
    //Close file stream
    outputStream.close();
    //.
}



#pragma mark Writing to file specific

void Experiment::write_toFile_table_latexTabularFormat_comparison() {
    using namespace std;
    
    //Creating file for writing
    string filePath;
    filePath.append(folderPath_);
    filePath.append(title_);
    filePath.append("/comparisonTableResult");
    filePath.append(".tex");
    ofstream outputStream(filePath.c_str(), ios_base::out | ios_base::trunc);
    //.
    
    
    
    outputStream << "\\begin{table}\n"
    "\\footnotesize\n"
    "\\centering\n"
    "\\begin{tabular}{c|c|c|c||r|r||c|c||c}\n"
    "\\hline\n"
    "C & N\\textsubscript{OI} & B & L & \\multicolumn{1}{c|}{B\\textsubscript{CPU}} & \\multicolumn{1}{c||}{B\\textsubscript{GPU}} & 99\\%\\textsubscript{CPU} &  99\\%\\textsubscript{GPU} & K \\\\ \n"
    "\\hline\n";
    
    std::string lastIRLength("");
    std::string lastNumOfChannels("");
    std::vector<TrialsSet>::iterator it_trialSet_CPU = trialsSets_.begin();
    std::vector<TrialsSet>::iterator it_trialSet_GPU = ++trialsSets_.begin();
    for (; it_trialSet_CPU != trialsSets_.end(); ++(++it_trialSet_CPU), ++(++it_trialSet_GPU)  ) {
        bool printedLine=false;
        if(( it_trialSet_CPU->properties_).get(Properties::num_of_channels).second != lastNumOfChannels ) {
            outputStream << "\\hline\n";
            printedLine = true;
            outputStream << "$";
            outputStream << (it_trialSet_CPU->properties_).get(Properties::num_of_channels).second;                          //IRLength
            outputStream << "$";
            lastNumOfChannels = (it_trialSet_CPU->properties_).get(Properties::num_of_channels).second;
        }
        
        if((it_trialSet_CPU->properties_).get(Properties::impulse_response_N).second != lastIRLength) {
            if(!printedLine) {
                outputStream << "\t\\cline{2-9}\n";
            }
            outputStream << "\t& ";
            printedLine = false;
            outputStream << "$";
            outputStream << (it_trialSet_CPU->properties_).get(Properties::impulse_response_N).second;                             //channelNumbers
            outputStream << "$";
            lastIRLength = (it_trialSet_CPU->properties_).get(Properties::impulse_response_N).second;
        }
        else
            outputStream << "\t& ";
        outputStream << "\t& ";

        
        outputStream << "$";
        outputStream << (it_trialSet_CPU->properties_).get(Properties::block_size).second;                                          //blockSize
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet_CPU->properties_).getStringFromFloatWithPrecision(Properties::latency, 2);                     //latency
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet_CPU->properties_).getStringFromFloatWithPrecision(Properties::onTimePercentage, 2);
        outputStream << "\\%$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet_GPU->properties_).getStringFromFloatWithPrecision(Properties::onTimePercentage, 2);
        outputStream << "\\%$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet_CPU->properties_).getStringFromFloatWithPrecision(Properties::_99, 2);                         //99 procent CPU
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet_GPU->properties_).getStringFromFloatWithPrecision(Properties::_99, 2);                         //99 procent GPU
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << getStringFromNumber(3, (boost::any_cast<double>((it_trialSet_CPU->properties_).get(Properties::_99).first))/(boost::any_cast<double>((it_trialSet_GPU->properties_).get(Properties::_99).first))  ) ;//GPU/CPU
        outputStream << "$";
        outputStream << "\\\\ \n";
    }
    //.
    
    std::vector<TrialsSet>::iterator it_trialSet = trialsSets_.begin();
    std::string lengthOfAudioInSeconds = (it_trialSet->properties_).get(Properties::lengthInSeconds).second;
    std::string sampleRate = (it_trialSet->properties_).get(Properties::sample_rate).second;
    std::string numberOfChannels = (it_trialSet->properties_).get(Properties::num_of_channels).second;
    
    
    
    
    outputStream << "\\hline\n"
    "\\end{tabular}\n"
    "\\caption{ Porównanie. Czas trwania pliku wejściowego: $" << lengthOfAudioInSeconds << "s$; częstotliwość próbkowania:" << sampleRate << "$Hz$. K oznacza  proporcję 99\\%\\textsubscript{CPU}/99\\%\\textsubscript{GPU} . }\n"
    "\\label{table:TYTUL}\n"
    "\\end{table}\n";
    
    
    //Close file stream
    outputStream.close();
    //.
}

void Experiment::write_toFile_table_latexTabularFormat_Max(bool ifGPUComputation) {
    using namespace std;
    
    std::string gpu("karcie graficznej");
    std::string cpu("procesorze CPU");
    std::string gpuA("GPU"), cpuA("CPU");
    std::string curr, currA;
    if(ifGPUComputation) {
        curr    = gpu;
        currA   = gpuA;
    }
    else {
        curr    =cpu;
        currA   =cpuA;
    }
    
    
    //Creating file for writing
    string filePath;
    filePath.append(folderPath_);
    filePath.append(title_);
    filePath.append("/generalTableReport_");
    filePath.append(currA);
    filePath.append(".tex");
    ofstream outputStream(filePath.c_str(), ios_base::out | ios_base::trunc);
    //.
    
    outputStream << "\\begin{table}\n"
                    "\\footnotesize\n"
                    "\\centering\n"
                    "\\begin{tabular}{c|c||c||c|c|c|c}\n"
                    "\\hline\n"
                    "B & N\\textsubscript{OI} & Maks kanałów & ŚredS";
    if(ifGPUComputation)
       outputStream << " & Ilość wątków & Obiegi Pętli & Pamięć";
    outputStream << " \\\\ \n\\hline\n";
    
    std::string lastBlockSize("");
    std::vector<TrialsSet>::iterator it_trialSet = trialsSets_.begin();
    for (; it_trialSet != trialsSets_.end(); ++it_trialSet ) {
        if((it_trialSet->properties_).get(Properties::block_size).second != lastBlockSize)  {
            outputStream << "\\hline\n";
            outputStream << "$";
            outputStream << (it_trialSet->properties_).get(Properties::block_size).second;                                      //blockSize
            outputStream << "$";
            lastBlockSize = (it_trialSet->properties_).get(Properties::block_size).second;
        }
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).get(Properties::latency).second;                                             //Latency
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).get(Properties::impulse_response_N).second;                                  //N_IR
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).get(Properties::num_of_channels).second;                                     //max numOfChannels
        outputStream << "$";
        outputStream << "\t& ";
        
        outputStream << "$";
        outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::averageTimePerSample, 2);        //averagePerSample
        outputStream << "$";
        
        if(ifGPUComputation) {
            outputStream << "\t\t& ";
            outputStream << "$";
            outputStream << (it_trialSet->properties_).get(Properties::gpuThreadsNumber).second;                                //gpuThreadsNumber
            outputStream << "$";
            outputStream << "\t\t& ";
            
            outputStream << "$";
            outputStream << (it_trialSet->properties_).get(Properties::num_mainKernel_LoopIteration).second;                    //loop iteration
            outputStream << "$";
            outputStream << "\t\t& ";
            
            outputStream << "$";
            outputStream << (it_trialSet->properties_).getStringFromFloatWithPrecision(Properties::allocatedGPUMemory, 0);      //gpu memory
            outputStream << "$";
        }
        outputStream << "\\\\ \n";

    }
    //.
    
    it_trialSet = trialsSets_.begin();
    std::string lengthOfAudioInSeconds = (it_trialSet->properties_).get(Properties::lengthInSeconds).second;
    std::string sampleRate = (it_trialSet->properties_).get(Properties::sample_rate).second;
    std::string blockSize = (it_trialSet->properties_).get(Properties::block_size).second;
    
    
    
    
    outputStream << "\\hline\n"
    "\\end{tabular}\n"
    "\\caption{ Wyniki czasowe. Czas trwania pliku wejściowego: " << lengthOfAudioInSeconds << "s; częstotliwość próbkowania: $" << sampleRate << "Hz$; obliczenia przeprowadzone na " << curr <<   ". }\n"
    "\\label{table:WynikiMax}\n"
    "\\end{table}\n";
    
    
    //Close file stream
    outputStream.close();
    //.
}





