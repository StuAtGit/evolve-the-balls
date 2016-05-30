#include <iostream>
#include <string>
#include "Timer.h"

/**
@file
Unit test for timer class
*/
using namespace std;

int main()
{
    Timer timer;
   
    cout << endl << "**Testing second count.. " << endl;
    cout << "Starting timers, then sleeping for three seconds..." << endl;
    timer.startTimer( 0 );
    timer.startTimer( 1 );
    timer.startTimer( 2 );
    timer.stopTimer( 0 );
    sleep( 3 );
    timer.stopTimer( 2 );
    timer.stopTimer( 1 );

    unsigned long long secondsZero =  timer.getSeconds( 0 );
    unsigned long long usecondsZero = timer.getMicroseconds( 0 );
    unsigned long long secondsTwo = timer.getSeconds( 2 );
    unsigned long long usecondsTwo = timer.getMicroseconds( 2 );
    unsigned long long secondsOneA = timer.getSeconds( 1 );
    unsigned long long usecondsOneA = timer.getMicroseconds( 1 );

    cout << "**After sleep for 3 seconds" << endl;
    cout << "Seconds for zero timer: " << secondsZero << endl;
    cout << "uSeconds for zero timer: " << usecondsZero << endl; 
    cout << "Seconds for one timer: " << secondsOneA << endl;
    cout << "uSeconds for one timer: " << usecondsOneA << endl;
    cout << "Seconds for two timer: " << secondsTwo << endl;
    cout << "uSeconds for two timer: " << usecondsTwo << endl;

    cout << endl << "**testing usleep.. start times:" << endl;
    cout << "Starting timers, then sleeping for three-hundred microseconds..." << endl;
    timer.startTimer( 0 );
    timer.startTimer( 1 );
    timer.stopTimer( 0 );
    timer.startTimer( 2 );

    usleep( 300 );

    timer.stopTimer( 2 );
    timer.stopTimer( 1 );
    secondsTwo = timer.getSeconds( 2 );
    usecondsTwo = timer.getMicroseconds( 2 );

    secondsOneA = timer.getSeconds( 1 );
    usecondsOneA = timer.getMicroseconds( 1 );

    cout << "**After usleep for 300 microseconds" << endl;
    cout << "Seconds for zero timer: " << secondsZero << endl;
    cout << "uSeconds for zero timer: " << usecondsZero << endl; 
    cout << "Seconds for one timer: " << secondsOneA << endl;
    cout << "uSeconds for one timer: " << usecondsOneA << endl;
    cout << "Seconds for two timer: " << secondsTwo << endl;
    cout << "uSeconds for two timer: " << usecondsTwo << endl;
    cout << "ids for all timers: ";
    vector<int> ids = timer.getTimerIds();
    for( unsigned int i = 0; i < ids.size(); ++i )
    {
        cout << ids[i] << " ";
    }

    cout << endl;

    cout << "stats: " << endl;

    timer.printStats();
}
