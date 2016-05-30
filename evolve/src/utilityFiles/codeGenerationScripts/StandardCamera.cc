#ifndef STU_StandardCamera_CC
#define STU_StandardCamera_CC

#include "StandardCamera.h"

StandardCamera::StandardCamera( SceneManager* sceneManager, RenderWindow* renderWindow )
{
    return;
}

bool StandardCamera::foward(  bool printDebug )
{
    logger.print( "Notify: entered method: bool foward( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::backward(  bool printDebug )
{
    logger.print( "Notify: entered method: bool backward( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::left(  bool printDebug )
{
    logger.print( "Notify: entered method: bool left( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::right(  bool printDebug )
{
    logger.print( "Notify: entered method: bool right( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::upward(  bool printDebug )
{
    logger.print( "Notify: entered method: bool upward( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::downward(  bool printDebug )
{
    logger.print( "Notify: entered method: bool downward( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::setSpeed( double speed, bool printDebug )
{
    logger.print( "Notify: entered method: bool setSpeed(double speed, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::setRotateSpeed( double speed, bool printDebug )
{
    logger.print( "Notify: entered method: bool setRotateSpeed(double speed, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::rotate( double x,double y, bool printDebug )
{
    logger.print( "Notify: entered method: bool rotate(double x,double y, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

map<string,Vector3> StandardCamera::getSelected( double clickX, double clickY, bool printDebug )
{
    logger.print( "Notify: entered method: map<string,Vector3> getSelected(double clickX, double clickY, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    map<string,Vector3> ret;
    return ret;
}

#endif
