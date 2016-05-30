#ifndef STU_GraphicalObject_CC
#define STU_GraphicalObject_CC

#include "GraphicalObject.h"

GraphicalObject::GraphicalObject()
{
    return;
}

bool GraphicalObject::setPosition( bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        logger.print( "Notify: entered method: bool setPosition");
    }
    return;
}

bool GraphicalObject::setPosition( bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        logger.print( "Notify: entered method: bool setPosition");
    }
    return;
}

bool GraphicalObject::setRotation( bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        logger.print( "Notify: entered method: bool setRotation");
    }
    return;
}

vector<double> GraphicalObject::getPosition( bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        logger.print( "Notify: entered method: vector<double> getPosition");
    }
    return;
}

double GraphicalObject::getRotation( bool printDebug )
{
    if( this->doNotify && printDebug )
    {
        logger.print( "Notify: entered method: double getRotation");
    }
    return;
}

#endif
