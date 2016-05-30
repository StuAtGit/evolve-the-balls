#ifndef STU_OIS_FRAME_H
#define STU_OIS_FRAME_H

#include <string>
#include <vector>
#include <map>
#include <iostream>

#include <Ogre.h>
#include <OgreException.h>
#include <OgreNode.h>

#include "OISMouse.h"
#include "OISKeyboard.h"
#include "OISInputManager.h"
//#include "LinuxMouse.h"

#include "CommonAspect.h"
#include "Utility.h"
#include "StandardCamera.h"
#include "PhysicsEngine.h"
#include "WorldObject.h"
#include "Timer.h"

using namespace std;
using namespace Ogre;
using namespace OIS;
using namespace evolve;

#define setBackgroundColor setBackgroundColour
#define ColorValue ColourValue
#define initialize initialise
#define initializeAllResourceGroups initialiseAllResourceGroups
#define setDiffuseColor setDiffuseColour
#define setSpecularColor setSpecularColour

namespace evolve
{
class OISFrame : public CommonAspect, public FrameListener
{
  public:
    OISFrame( Root* root, SceneManager* sceneManager, PhysicsEngine* physicsEngine );
    virtual ~OISFrame();
    virtual bool addWorldObjects( map<string,WorldObject*> objects );
    virtual bool frameStarted( const FrameEvent &evt );
    virtual bool frameEnded( const FrameEvent &evt );
    /**
     * Profiling function that returns the amount of time
     * spent doing physics
     * @returns the number of seconds spent doing physics.
     *          timer granularity is microseconds.
     */
    double getPhysicsTime();
    double getOgreTime();
    double getFrameTime();
    const StandardCamera* getCamera();

  protected:
    virtual bool handleKeyPressed();
    virtual bool handleKeyReleased();
    virtual bool handleMouseMotion();
    void setWindowExtents( int width, int height );

  private:
    Timer timer;
    Keyboard* keyboard;
    Mouse* mouse;
    InputManager* inputManager;
    SceneManager* sceneManager;
    RenderWindow* window;
    StandardCamera* camera;
    PhysicsEngine* physicsEngine;
    map<string,WorldObject*> worldObjects;
    unsigned int width;
    unsigned int height;
    //whether mouse motion moves the camera...
    bool move;
    double physicsTime;
    double ogreTime;
    double frameTime;
};
};
#endif
