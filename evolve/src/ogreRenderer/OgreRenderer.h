#ifndef OGRE_RENDERER_H_STU
#define OGRE_RENDERER_H_STU

#include <string>
#include <vector>
#include <map>
#include <ostream>

#include "Ogre.h"
#include "OgreException.h"

#include "OgreGDKCodec.h"
#include "OISFrame.h"

#include "Utility.h"
#include "Timer.h"
#include "CommonAspect.h"
#include "PhysicsEngine.h"
#include "WorldObject.h"


using namespace std;
using namespace Ogre;

#define setBackgroundColor setBackgroundColour
#define ColorValue ColourValue
#define initialize initialise
#define initializeAllResourceGroups initialiseAllResourceGroups
#define setDiffuseColor setDiffuseColour
#define setSpecularColor setSpecularColour


/*
 * Responsible for rendering and updating all the world objects.
 * Have pondered the need for an update callback that is handed 
 * out by the frame (and updates would be checked for every frameStarted()).
 * But for now, we'll assume WorldObjects will know how to morph themselves
 * over time (in update())
 */
class OgreRenderer : public CommonAspect
{
  public:
    OgreRenderer();
    virtual ~OgreRenderer();
    virtual bool createScene( const map<string,WorldObject*>& objects );
    virtual SceneManager* getSceneManager();
    virtual PhysicsEngine* getPhysicsEngine();
    virtual OISFrame* getFrame();
    virtual bool run();
    /**
     * Sets the gravity vector in the physics engine.
     * @param grav the vector
     */
    virtual bool setGravity( vector<double> grav );
    /**
     * Diagnostic method
     * @returns number of seconds spent doing physics
     *          granularity is in microseconds
     */
    double getPhysicsTime();
  protected:
    void setupResources(); 
    OISFrame* frame;
    Root* root;
    SceneManager* sceneManager;
    bool setupSuccessful;
    PhysicsEngine* physicsEngine;
    map<string,WorldObject*> worldObjects;
};

#endif
