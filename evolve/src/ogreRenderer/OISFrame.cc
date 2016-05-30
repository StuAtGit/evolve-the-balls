#ifndef STU_OIS_FRAME_CC
#define STU_OIS_FRAME_CC

#include "OISFrame.h"

//Eventually, we pass this the sceneManager, so we can create a camera
OISFrame::OISFrame( Root* root, SceneManager* sceneManager, PhysicsEngine* physicsEngine )
{
    this->width = 1024;
    this->height = 768;
    this->physicsTime = 0;
    this->ogreTime = 0;
    this->frameTime = 0;
    unsigned int windowHandle = 0;
    bool fullscreen = false;

    map<string,string> options;
    options["title"] = "Stu";

    //Set up the OGRE variables
    this->window = root->createRenderWindow( "ogre", this->width, this->height, fullscreen, &options );
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    this->sceneManager = sceneManager;
    this->physicsEngine = physicsEngine;

    //Set up our input system
    window->getCustomAttribute( "WINDOW", &windowHandle );
    ostringstream ostream;
    ostream << windowHandle;
    OIS::ParamList parameterList;
    parameterList.insert( make_pair( "WINDOW", ostream.str() ) );

    /**
     * makes things work for mouse motion but then alt-tab breaks.. why? why? pjcast.
     * I just need a warp pointer that works!
     * figure out how to throw alt-tab signal up to x & not hoard it anymore
     */
    parameterList.insert(std::make_pair( "x11_mouse_grab", "true" ) );
    parameterList.insert(std::make_pair( "x11_mouse_hide", "true" ) );
    parameterList.insert(std::make_pair( "x11_keyboard_grab", "false" ) );
    parameterList.insert(std::make_pair( "XAutoRepeatOn", "true" ) );

    this->camera = new StandardCamera( this->sceneManager, this->window );
    this->camera->move( Vector3(0, 250, -100) );
    this->inputManager = InputManager::createInputSystem( parameterList );
    this->keyboard = static_cast<Keyboard*>( this->inputManager->createInputObject( OISKeyboard, false ) );
    this->mouse = static_cast<Mouse*>( this->inputManager->createInputObject( OISMouse, false ) );
    this->move = true;
}

OISFrame::~OISFrame() 
{
    if( this->keyboard )
    {
        this->inputManager->destroyInputObject( this->keyboard );
    }
    if( this->mouse )
    {
        this->inputManager->destroyInputObject( this->mouse );
    }
    this->inputManager->destroyInputSystem( this->inputManager );
    this->inputManager = NULL;
    this->keyboard = NULL;

}

bool OISFrame::addWorldObjects( map<string,WorldObject*> objects )
{
    this->worldObjects = objects;
    return true;
}

bool OISFrame::frameStarted( const FrameEvent &evt )
{
    //if ( this->timer.stopTimer(0) )
    //    this->ogreTime += this->timer.getMicroseconds(0) * 1.e-6;
    //this->timer.startTimer(51);
    bool continueSimulation = handleKeyPressed();
    handleMouseMotion();
    //this->timer.startTimer(42);
    //this->physicsEngine->doPhysics();
    //this->timer.stopTimer(42);
    //long long useconds = this->timer.getMicroseconds( 42 );
    //this->physicsTime += useconds * (1.e-6);

    for( map<string,WorldObject*>::iterator i = this->worldObjects.begin(); i != this->worldObjects.end(); ++i )
    {
        (*i).second->grow();
    }
    //this->timer.stopTimer(51);
    //this->frameTime += ( this->timer.getMicroseconds( 51 ) * (1.e-6) );
    //this->timer.startTimer(0);
    return continueSimulation;
}

bool OISFrame::frameEnded( const FrameEvent &evt )
{
    //if( this->timer.stopTimer(0) )
    //    this->ogreTime += this->timer.getMicroseconds(0) * 1.e-6;
    //time since last frame doesn't really help..
    this->camera->updatePosition( evt.timeSinceLastFrame );
    //this->timer.startTimer(0);
    return true;
}

bool OISFrame::handleKeyPressed()
{
    if( !this->keyboard )
    {
        return false;
    }

    this->keyboard->capture();
    ( this->keyboard->isKeyDown( OIS::KC_W ) ) &&
        this->camera->forward();

    ( this->keyboard->isKeyDown( OIS::KC_S ) ) &&
        this->camera->backward();

    ( this->keyboard->isKeyDown( OIS::KC_A ) ) &&
        this->camera->left();

    ( this->keyboard->isKeyDown( OIS::KC_D ) ) &&
        this->camera->right();
    
    ( this->keyboard->isKeyDown( OIS::KC_Q ) ) &&
        this->camera->up();
    
    ( this->keyboard->isKeyDown( OIS::KC_E ) ) &&
        this->camera->down();
    
    if( this->keyboard->isKeyDown( OIS::KC_ESCAPE ) )
    {
        return false;
    }

    if( 
        this->keyboard->isKeyDown( OIS::KC_F4 ) &&
        this->keyboard->isModifierDown( OIS::Keyboard::Alt )
      )
      {
        return false;
      }

    return true;
}

bool OISFrame::handleKeyReleased()
{
    return false;
}

bool OISFrame::handleMouseMotion()
{
    this->mouse->capture();
    MouseState mState = this->mouse->getMouseState();
    if( mState.X.rel != 0 || mState.Y.rel != 0 )
    {
        this->camera->rotate( mState.X.rel, mState.Y.rel );
        //dynamic_cast<LinuxMouse*>(this->mouse)->warpPointer( this->width / 2, this->height / 2 );
    }
    return true;
}

double OISFrame::getPhysicsTime()
{
    return this->physicsTime;
}

double OISFrame::getOgreTime()
{
    return this->ogreTime;
}

double OISFrame::getFrameTime()
{
    return this->frameTime;
}

const StandardCamera* OISFrame::getCamera()
{
    return this->camera;
}

#endif
