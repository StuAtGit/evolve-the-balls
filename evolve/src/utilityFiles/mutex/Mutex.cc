#ifndef STU_Mutex_CC
#define STU_Mutex_CC

#include "Mutex.h"

Mutex::Mutex()
{
    int status = pthread_mutexattr_init( &(this->attr) );
    logger.print( "Warning: mutex attr init failed: " + Utility::toString( strerror(status) ), status != 0 );
    pthread_mutex_init( &(this->mutex), &(this->attr) );
    logger.print( "Warning: mutex init failed: " + Utility::toString( strerror(status) ), status != 0 );
}

Mutex::~Mutex()
{
    int status = pthread_mutex_destroy( &(this->mutex) );
    logger.print( "Warning: mutex destroy failed: " + Utility::toString( strerror(status) ), status != 0 );
    status = pthread_mutexattr_destroy( &(this->attr) );
    logger.print( "Warning: mutex attr destroy failed: " + Utility::toString( strerror(status) ), status != 0 );
}

bool Mutex::lock( bool printDebug )
{
    logger.print( "Notify: entered method:  lock( bool printDebug)  of class Mutex", this->doNotify && printDebug );
    int status = pthread_mutex_lock( &(this->mutex) );
    logger.print( "Warning: mutex lock failed with error: " + 
                   lexical_cast<string>(strerror(status)), status != 0 && printDebug );
    return status == 0;
}

bool Mutex::unlock( bool printDebug )
{
    logger.print( "Notify: entered method:  unlock( bool printDebug)  of class Mutex", this->doNotify && printDebug );
    int status = pthread_mutex_unlock( &(this->mutex) );
    logger.print( "Warning: mutex lock failed with error: " + 
                   lexical_cast<string>(strerror(status)), status != 0 && printDebug );
    return status == 0;
}

#endif
