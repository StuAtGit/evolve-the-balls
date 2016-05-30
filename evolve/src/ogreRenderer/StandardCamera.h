#ifndef STU_StandardCamera_H
#define STU_StandardCamera_H

#include <string>
#include <vector>
#include <map>

#include "Ogre.h"
#include "OgreException.h"

#include "CommonAspect.h"
#include "Utility.h"

#define DO_DEBUG true

#define setBackgroundColor setBackgroundColour
#define ColorValue ColourValue
#define initialize initialise
#define initializeAllResourceGroups initialiseAllResourceGroups
#define setDiffuseColor setDiffuseColour
#define setSpecularColor setSpecularColour

using namespace std;
using namespace boost;
using namespace Ogre;
using namespace evolve;

namespace evolve
{
/**
 * Wrapper around ogre camera which methods to make it behave in a "standard"
 * fashion based on the given user input. "Standard" is defined according to me.
 * It means a mouse motion event (given in x and y) moves where the camera looks.
 * forward/upward/etc should be obvious, except for one caveat: motion is always relative
 * to where the camera is looking (not as in FPS games). If your looking up, and move
 * forward, you move up.
 * mousePressed/mouseReleased is used to track the selection/dropping of objects at a given
 * x/y.
 * */
class StandardCamera : public CommonAspect
{
public:
    StandardCamera( SceneManager* sceneManager, RenderWindow* renderWindow, string name = "StandardCamera" );
    virtual ~StandardCamera() {};
    /**
    *@param time the time since the last update (unit agnostic, but should be consistent)
    */
    virtual bool updatePosition( double timeSinceLastFrame, bool printDebug = DO_DEBUG );
    /**
    * @param moveVector the vector to translate the camera by
    */
    virtual bool move( Vector3 moveVector, bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool forward( bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool backward( bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool left( bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool right( bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool up( bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool down( bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool setSpeed( double speed, bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool setRotateSpeed( double speed, bool printDebug = DO_DEBUG );
    /**
    *
    */
    virtual bool rotate( double x, double y, bool printDebug = DO_DEBUG );
    /**
     */
    virtual bool mousePressed( double clickX, double clickY, bool printDebug = DO_DEBUG ); 
    /**
    * Returns the name and new position of all previously selected movable objects
    * the name is the same name as given by Ogre::movable->getName()
    * the location is the new location in three space (where it would be "dropped")
    * @param clickX the x location of where the mouse was clicked
    * @param clickY the y location of where the mouse was clicked
    */
    virtual map<string,Vector3> mouseReleased( double clickX, double clickY, bool printDebug = DO_DEBUG );
private:
    Camera* camera;
    SceneManager* sceneManager;
    RenderWindow* window;
    Vector3 moveVector;
    double speed;
    double rotateSpeed;
};
};
#endif
