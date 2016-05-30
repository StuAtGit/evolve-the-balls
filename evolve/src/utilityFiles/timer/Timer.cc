#ifndef STU_TIMER_CC
#define STU_TIMER_CC

#include "Timer.h"

    
Timer::Timer()
{
    doNotify = false;
}

bool Timer::startTimer( int referenceId, bool printDebug )
{
    if( printDebug && doNotify )
    {
        logger.print( "Notify: entered start timer" );
    }
    
    timeval startTime;
    if( gettimeofday( &startTime, NULL ) < 0 )
    {
        if( printDebug )
        {
            logger.print( "Error: call to gettimeofday failed" );
        }
        return false;
    }
    
    times[referenceId].tv_sec = startTime.tv_sec;
    times[referenceId].tv_usec = startTime.tv_usec;
    stopped[referenceId] = false;

    return true;
}

bool Timer::stopTimer( int referenceId, bool printDebug )
{
    if( printDebug && doNotify )
    {
        logger.print( "Notify: entered stopTimer" );
    }
    
    if( times.find( referenceId ) == times.end() )
    {
        if( printDebug )
        {
            logger.print( "Warning: attempted to stop a timer that was never started" );
        }
        return false;
    }

    timeval endTime;
    if( gettimeofday( &endTime, NULL ) < 0 )
    {
        if( printDebug )
        {
            logger.print( "Error: gettimeofday failed" );
        }
        return false;
    }

    times[referenceId].tv_sec = endTime.tv_sec - times[referenceId].tv_sec;
    times[referenceId].tv_usec = endTime.tv_usec - times[referenceId].tv_usec;

    stopped[referenceId] = true;

    return true;
}

long long Timer::getSeconds( int referenceId, bool printDebug )
{
    if( printDebug && doNotify )
    {
        logger.print( "Notify: entered getSeconds" );
    }
    
    if( times.find( referenceId ) == times.end() )
    {
        if( printDebug )
        {
            logger.print( "Warning: attempted to check a timer that was never created" );
        }
        return -1;
    }

    if( stopped[referenceId] )
    {
        return times[referenceId].tv_sec;
    }
    else
    {
        timeval time;
        if( gettimeofday( &time, NULL ) < 0 )
        {
            if( printDebug )
            {
                logger.print( "Error: gettimeofday failed" );
            }
            return -1;
        }

        return time.tv_sec - times[referenceId].tv_sec;
    }

    
}

long long Timer::getMicroseconds( int referenceId, bool printDebug )
{
    if( printDebug && doNotify )
    {
        logger.print( "Notify: entered getMicroseconds" );
    }
    
    if( times.find( referenceId ) == times.end() )
    {
        if( printDebug )
        {
            logger.print( "Warning: attempted to check a timer that was never created" );
        }
        return -1;
    }

    if( stopped[referenceId] )
    {
        return times[referenceId].tv_usec + times[referenceId].tv_sec * 1000000;
    }
    else
    {
        timeval time;
        if( gettimeofday( &time, NULL ) < 0 )
        {
            if( printDebug )
            {
                logger.print( "Error: gettimeofday failed" );
            }
            return -1;
        }

        return (time.tv_usec - times[referenceId].tv_usec) + (time.tv_sec - times[referenceId].tv_sec) * 1000000;
    }
}

bool Timer::printStats( bool printDebug )
{
    if( printDebug && doNotify )
    {
        logger.print( "Notify: entered printStats" );
    }

    for( map<int,timeval>::iterator i = times.begin(); i != times.end(); ++i )
    {
        cout << "***Timer: " << (*i).first << endl;
        cout << "Seconds: " << getSeconds( (*i).first ) << endl;
        cout << "Microseconds: " << getMicroseconds( (*i).first ) << endl;
    }

    return true;
}

vector<int> Timer::getTimerIds( bool printDebug )
{
    vector<int> ret;

    for( map<int,timeval>::iterator i = times.begin(); i != times.end(); ++i )
    {
        ret.push_back( (*i).first );
    }

    return ret;
}

#endif
