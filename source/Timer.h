#ifndef __LowPassFilter_Standalone__Timer__
#define __LowPassFilter_Standalone__Timer__

#include <sys/time.h>
#include "GeneralExceptions.h"
#include <vector>

class SimplyTimer {
public:
    void start() {
        start_ = getRealTime();
    }
    void stop() {
        stop_ = getRealTime();
    }
    double getTime_in_ms() {
        return 1000 * (stop_-start_);
    }
    
private:
    double getRealTime() {
        struct timeval tv;
        gettimeofday(&tv,0);
        return (double)tv.tv_sec+1.0e-6*(double)tv.tv_usec;
    }
    double start_, stop_;
};

//gives time in ms
class Timer {
public:
    enum TimerID {
         MAIN_TIMER = 0,
        TRIAL_TIMER = 1
    };
    
    //start, stop
    void start(TimerID timerID) {
        switch (timerID) {
            case MAIN_TIMER:
                mainTimer_.start();
                break;
                
            case TRIAL_TIMER:
                trialTimer_.start();
                break;
                
            default:
                /*empty*/;
        }
    }
    void stop (TimerID timerID) {
        switch (timerID) {
            case MAIN_TIMER:
                mainTimer_.stop();
                mainTiming_ = mainTimer_.getTime_in_ms();
                break;
                
            case TRIAL_TIMER:
                trialTimer_.stop();
                trialsTimings_.push_back(trialTimer_.getTime_in_ms());
                break;
                
            default:
                /*empty*/;
        }
    }
    //.
    
    //get
    double              get_mainTiming   () { return mainTiming_; }
    std::vector<double> get_trialsTimings() { return trialsTimings_; }
    //.

private:
    SimplyTimer mainTimer_;
    SimplyTimer trialTimer_;
    
    double mainTiming_;
    std::vector<double> trialsTimings_;
};


#endif /* defined(__LowPassFilter_Standalone__Timer__) */
