#ifndef Stu_ReadWriteLock_CC
#define Stu_ReadWriteLock_CC

#include "ReadWriteLock.h"

ReadWriteLock::ReadWriteLock()
{
    int ret = 0;
    pthread_rwlockattr_t attr;
    this->lock = (pthread_rwlock_t*)malloc( sizeof(pthread_rwlock_t) ); 

    if( ( ret = pthread_rwlockattr_init( &attr ) ) != 0 ||
        ( ret = pthread_rwlock_init( this->lock, &attr ) ) != 0 )
    {
        logger.print( "Error: ReadWriteLock::ReadWriteLock initialization of read write lock failed: " + lexical_cast<string>( strerror( ret ) ), true );
        this->initialized = false;
    }
    this->initialized = true;
    return;
}

ReadWriteLock::~ReadWriteLock()
{
    if( this->initialized )
    {
        pthread_rwlock_destroy( this->lock );
    }
    //I saw this cause double-free's with mutexes getting allocated in maps...
    //just leak it until I figure out what happened with that
    //(I need a better understanding how map allocation & thread destruction stuff interacts. 
    //free( this->lock );
    return;
}

bool ReadWriteLock::readLock(  bool printDebug )
{
    logger.print( "Notify: entered method: bool readLock( bool printDebug)  of class ReadWriteLock", this->doNotify && printDebug );
    if( !this->initialized ) 
    {
        logger.print( "Warning: ReadWriteLock::readLock called readLock, but intialization of object failed", printDebug );
        return false;
    }
    int ret = 0;
    ret = pthread_rwlock_rdlock( this->lock );
    logger.print( "Error: ReadWriteLock::readLock failed: " + lexical_cast<string>( strerror( ret ) ), ret != 0 && printDebug );
    return ret == 0;
}

bool ReadWriteLock::writeLock(  bool printDebug )
{
    logger.print( "Notify: entered method: bool writeLock( bool printDebug)  of class ReadWriteLock", this->doNotify && printDebug );
    if( !this->initialized ) 
    {
        logger.print( "Warning: called ReadWriteLock::writeLock, but intialization of object failed", printDebug );
        return false;
    }
    int ret = 0;
    ret = pthread_rwlock_wrlock( this->lock );
    logger.print( "Error: ReadWriteLock::writeLock failed: " + lexical_cast<string>( strerror( ret ) ), ret != 0 && printDebug );
    return ret == 0;
}

bool ReadWriteLock::unlock(  bool printDebug )
{
    logger.print( "Notify: entered method: bool unlock( bool printDebug)  of class ReadWriteLock", this->doNotify && printDebug );
    if( !this->initialized ) 
    {
        logger.print( "Warning: called ReadWriteLock::unlock, but intialization of object failed", printDebug );
        return false;
    }
    int ret = 0;
    ret = pthread_rwlock_unlock( this->lock );
    logger.print( "Error: ReadWriteLock::unlock failed: " + lexical_cast<string>( strerror( ret ) ), ret != 0 && printDebug );
    return ret == 0;
}

#endif
