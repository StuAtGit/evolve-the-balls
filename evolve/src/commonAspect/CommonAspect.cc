#ifndef STU_CommonAspect_CC
#define STU_CommonAspect_CC

#include "CommonAspect.h"
CommonAspect::CommonAspect()
{
    this->doNotify = false;
}

bool CommonAspect::setNotify( bool notify, bool printDebug )
{
    //believe it or not, g++ made code that got this far....
    if( this == NULL )
    {
        cout << "Warning: in CommonAspect::setNotify() this is null! (you called setNotify on a null object)" << endl;
        return false;
    }
    this->logger.print( "Notify: in CommonAspect::setNotify", printDebug && this->doNotify );
    this->doNotify = notify;
    return true;
}

bool CommonAspect::getNotify( bool printDebug )
{
    this->logger.print( "Notify: in CommonAspect::getNotify", printDebug && this->doNotify );
    return this->doNotify;
}

bool CommonAspect::getLogger( Logger*& loggerReturn, bool printDebug )
{
    this->logger.print( "Notify: in CommonAspect::getLogger", printDebug && this->doNotify );
    if( loggerReturn == NULL )
    {
        delete loggerReturn;
    }
    
    loggerReturn = &(this->logger);
    return true;
}

bool CommonAspect::serialize( unsigned char* buf, unsigned int& size )
{
    this->logger.print( "Warning: in CommonAspect::serialize, attempted to serialize an object that has not implemented serialization" );
    /*hahaha you should have implemented this anyways...!*/
    if( buf != NULL )
    {
        delete buf;
        buf = NULL;
    }
    size = 0;
    return false;
}

bool CommonAspect::deserialize( const unsigned char* buf, unsigned int size )
{
    this->logger.print( "Warning: in CommonAspect::deserialize, attempted to deserialize an object that has not implemented deserialization" );
    return false;
}

#endif
