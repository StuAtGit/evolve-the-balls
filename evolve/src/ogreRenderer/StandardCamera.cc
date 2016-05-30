#ifndef STU_StandardCamera_CC
#define STU_StandardCamera_CC

#include "StandardCamera.h"

StandardCamera::StandardCamera( SceneManager* sceneManager, RenderWindow* renderWindow, string name )
{
    this->sceneManager = sceneManager;
    this->window = renderWindow;

    this->camera = sceneManager->createCamera( name );
    this->camera->setPosition( Vector3( 0,0,500 ) );
    this->camera->lookAt( Vector3( 0,0,0 ) );
    this->camera->setNearClipDistance( 5 );

    string cameraNodeName = name + "Node";
    SceneNode* cameraNode = sceneManager->getRootSceneNode()->createChildSceneNode( cameraNodeName, Vector3( 0, 0, 500 ) ); 
    cameraNode->attachObject( camera );

    Viewport* viewport = this->window->addViewport( this->camera );
    viewport->setBackgroundColor( ColorValue( 0,0,0 ) );
    this->camera->setAspectRatio( viewport->getActualWidth() / viewport->getActualHeight() );
    this->camera->setPosition( Vector3( 0, 0, 500 ) );
    this->camera->lookAt( Vector3( 0, 0, 0 ) );
    this->camera->setNearClipDistance( 5 );
    this->speed = 1;
    this->rotateSpeed = 1;
    this->moveVector = Vector3::ZERO;
    return;
}

bool StandardCamera::updatePosition( double time, bool printDebug )
{
    ( this->moveVector != Vector3::ZERO ) && this->move( this->moveVector * ( this->speed * 1.0/time ) );
    this->moveVector = Vector3::ZERO;
    return true;
}

bool StandardCamera::move( Vector3 move, bool printDebug )
{
    this->camera->move( move );
    return true;
}

bool StandardCamera::forward( bool printDebug )
{
    logger.print( "Notify: entered method: bool foward( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    Vector3 up = this->camera->getRealUp();
    Vector3 right = this->camera->getRealRight();
    Vector3 forward = up.crossProduct( right );
    this->moveVector += forward;
    return true ;
}

bool StandardCamera::backward(  bool printDebug )
{
    logger.print( "Notify: entered method: bool backward( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    Vector3 up = this->camera->getRealUp();
    Vector3 right = this->camera->getRealRight();
    Vector3 backward = up.crossProduct( right ) * -1;
    this->moveVector += backward;
    return true ;
}

bool StandardCamera::left(  bool printDebug )
{
    logger.print( "Notify: entered method: bool left( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    Vector3 left = this->camera->getRealRight() * -1;
    this->moveVector += left;
    return true ;
}

bool StandardCamera::right(  bool printDebug )
{
    logger.print( "Notify: entered method: bool right( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    Vector3 right = this->camera->getRealRight();
    this->moveVector += right;
    return true;
}

bool StandardCamera::up(  bool printDebug )
{
    logger.print( "Notify: entered method: bool upward( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    Vector3 up = this->camera->getRealUp();
    this->moveVector += up;
    return true;
}

bool StandardCamera::down(  bool printDebug )
{
    logger.print( "Notify: entered method: bool downward( bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    Vector3 down = this->camera->getRealUp() * -1;
    this->moveVector += down;
    return true;
}

bool StandardCamera::setSpeed( double speed, bool printDebug )
{
    logger.print( "Notify: entered method: bool setSpeed(double speed, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    this->speed = speed;
    return true;
}

bool StandardCamera::setRotateSpeed( double speed, bool printDebug )
{
    logger.print( "Notify: entered method: bool setRotateSpeed(double speed, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;
}

bool StandardCamera::rotate( double x, double y, bool printDebug )
{
    logger.print( "Notify: entered method: bool rotate(double x,double y, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    this->camera->yaw( Degree( -this->rotateSpeed * x ) );
    this->camera->pitch( Degree( -this->rotateSpeed * y ) );
    return true;
}

bool StandardCamera::mousePressed( double clickX, double clickY, bool printDebug )
{
    logger.print( "Notify: entered method: bool mousePressed(double clickX, double clickY, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    bool ret;
    return ret;

}

map<string,Vector3> StandardCamera::mouseReleased( double clickX, double clickY, bool printDebug )
{
    logger.print( "Notify: entered method: map<string,Vector3> getSelected(double clickX, double clickY, bool printDebug)  of class StandardCamera", this->doNotify && printDebug );
    map<string,Vector3> ret;
    return ret;
}

#endif
