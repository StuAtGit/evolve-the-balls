#ifndef STU_SDLFRAME_H
#define STU_SDLFRAME_H

#include <string>
#include <vector>
#include <map>
#include <ostream>


#include "Ogre.h"
#include "OgreStringConverter.h"
#include "OgreException.h"

//#include "ExampleApplication.h"
#include "Utility.h"
#include "CommonAspect.h"
#include "PhysicsEngine.h"

namespace _SDL_
{
    #include <X11/Xlib.h>
    #include <X11/keysym.h>
    #include <X11/Xutil.h>
    #include <SDL/SDL.h>
    #include <SDL/SDL_syswm.h>
};

#define setBackgroundColor setBackgroundColour
#define ColorValue ColourValue
#define initialize initialise
#define initializeAllResourceGroups initialiseAllResourceGroups
#define setDiffuseColor setDiffuseColour
#define setSpecularColor setSpecularColour

using namespace std;
using namespace Ogre;
using namespace _SDL_;

class SDLFrame : public FrameListener, public CommonAspect
{
public:
    SDLFrame( Root* root, SceneManager* sceneManager, PhysicsEngine* physicsEngine );
    virtual ~SDLFrame();
    virtual bool frameStarted( const FrameEvent &evt );
    virtual bool frameEnded( const FrameEvent &evt );

protected:
    /**
     * update the ogre debug stats FPS, etc
     */
    void updateStats();
    /**
     * keyboard event handler
     */
    virtual bool handleKeyPress( SDL_Event event );
    /**
     * mouse clicked event handler
     */
    virtual bool handleMouseClicked( SDL_Event event );
    /**
     * keyboard event handler
     */
    virtual bool handleKeyReleased( SDL_Event event );
    /**
     * mouse event handler
     */
    virtual bool handleMouseMove( SDL_Event event );
    /**
     * Set's up our SDL window to cooperate with the ogre window.
     * Impelements the SDL window id hack.
     * See: http://www.ogre3d.org/wiki/index.php/Using_SDL_Input
     */
    void initializeSDLWindow();
    /**
     * reads the config file and sets width, height, fullscreen
     * @param config Represents the values in the config file
     * @param width set to the width in the config variable
     * @param height set to the height in the config variable
     * @param fullscreen set according to the fullscreen flag in the config variable 
     */
    void parseWindowGeometry( Ogre::ConfigOptionMap& config, unsigned int& width, unsigned int& height, bool& fullscreen );

    Real rotateDistance;
    //gets multipled by time since time since last frame
    Real moveRate;
    Vector3 moveVector;
    Overlay* debugOverlay;
    Camera* camera;
    RenderWindow* window;
    Root* root;
    SceneManager* sceneManager;
    RaySceneQuery* raySceneQuery;
    PhysicsEngine* physicsEngine;

    string cameraNodeName;
    bool continueSimulation;
    //whether the mouse moves the camera or the cursor
    bool mouseMoveCamera;
    unsigned int width;
    unsigned int height;
    //versus set by WarpMouse call that re-centers the cursor
    bool cursorSetByUser;
    bool move;
    //a list of the objects currently selected
    vector<MovableObject*> objectsSelected;
};

#endif
