#ifndef STU_TIMER_H
#define STU_TIMER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <sys/time.h>
#include "Logger.h"
#include "CommonAspect.h"

using namespace std;
using namespace evolve;
#define DO_DEBUG true

namespace evolve
{

class Timer : public CommonAspect
{
public:
    Timer();
    /**
    Starts a timer with a given reference id
    @param referenceId An id to refer to this particular timer
    */
    bool startTimer( int referenceId, bool printDebug = DO_DEBUG );
    /**
    Stops a timer with a given reference id
    @param referenceId An id to refer to a particular timer
    */
    bool stopTimer( int referenceId, bool printDebug = DO_DEBUG );
    /**
    Gets the number of seconds passed on a given timer
    @param referenceId An id to refer to a particular timer
    @return the number of seconds passed as a long long (so it can handle all valid values & -1)
    */
    long long getSeconds( int referenceId, bool printDebug = DO_DEBUG );
    /**
    Gets the number of microseconds passed on the given timer
    @param referenceId An id to refer to a particular timer
    @return the number of microseconds passed as a long long (so it can handle all valid values & -1)
    */
    long long getMicroseconds( int referenceId, bool printDebug = DO_DEBUG );
    /**
    Prints stats on all timers
    */
    bool printStats( bool printDebug = DO_DEBUG );
    /**
    Gets a list of the ids for all available timers.
    Useful if timers are started running automatically
    @return a vector of all the ids of existing timers
    */
    vector<int> getTimerIds( bool printDebug = DO_DEBUG );
    
private:
    map<int, timeval> times;
    map<int, bool> stopped;
};

};
#endif
